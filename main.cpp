#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <csignal>
#include <sstream>
#include <iomanip>


volatile std::sig_atomic_t signalReceived = 0;

void signalHandler(int signal) {
    signalReceived = 1;
}

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    // Format: YYYY-MM-DD HH:MM
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M"); 
    // Format: YYYY-MM-DD HH:MM:SS
    // ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}


void addDateTimeToFrame(cv::Mat &frame) {
    std::string dateTime = getCurrentDateTime();
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.5;
    int thickness = 3;
    cv::Point textOrg(frame.cols - 1200, 50); // Adjust based on frame size or desired location
    cv::Scalar textColor(0, 255, 255); // Yellow text color

    std::string description = "Any words you want to add..." + dateTime;
    cv::putText(frame, description, textOrg, fontFace, fontScale, textColor, thickness);
}

bool decisionHardThreshold(double totalKineticEnergy, double kineticEnergyThreshold) {

    return totalKineticEnergy > kineticEnergyThreshold;
}

int main(int, char**) {
    // Register signal handler
    std::signal(SIGINT, signalHandler);

    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Cannot open the camera" << std::endl;
        return -1;
    }

    cv::Mat prevGray, frame, gray, flow, flowParts[2], magnitude, kineticEnergy;
    cap >> frame; // get the first frame to initialize the matrices
    cv::cvtColor(frame, prevGray, cv::COLOR_BGR2GRAY);
    double kineticEnergyThreshold = 1000; // Set according to your needs
    int recordTime = 30 * 1000; // Record for 30 seconds after detection
    bool saveVideo = false;
    cv::VideoWriter writer;

    auto start = std::chrono::steady_clock::now();
    int countdown = 0; // Time remaining to record after detection

    while(true) {
        if (signalReceived) {
            std::cout << "Signal received, shutting down." << std::endl;
            break;
        }

        cap >> frame;
        if (frame.empty()) break;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Compute the optical flow
        cv::calcOpticalFlowFarneback(prevGray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        cv::split(flow, flowParts);

        // Compute the kinetic energy for each pixel and the total kinetic energy
        cv::magnitude(flowParts[0], flowParts[1], magnitude);
        cv::pow(magnitude, 2, kineticEnergy);
        double totalKineticEnergy = std::sqrt(cv::sum(kineticEnergy)[0]);

        std::cout << totalKineticEnergy  << std::endl;
        if (decisionHardThreshold(totalKineticEnergy, kineticEnergyThreshold)) {
            if (!saveVideo) {
                std::string filename = "intrusion_" + std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) + ".avi";
                writer.open(filename, cv::VideoWriter::fourcc('M','J','P','G'), 10, frame.size(), true);
                if (!writer.isOpened()) {
                    std::cerr << "Could not open the output video file for write\n";
                    return -1;
                }
                saveVideo = true;
                std::cout << "Intrusion detected: Starting recording." << std::endl;
            }
            countdown = recordTime;
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        start = now; // Reset start time for next loop iteration

        if (saveVideo) {
            addDateTimeToFrame(frame);
            writer.write(frame);
            countdown -= elapsed;
            std::cout << "elapsed: " << elapsed << std::endl;
            std::cout << "countdown: " << countdown << std::endl;

            if (countdown <= 0) {
                saveVideo = false;
                writer.release();
                std::cout << "Stopping recording due to timeout." << std::endl;
            }
        }

        // Update the previous frame
        gray.copyTo(prevGray);
    }

    cap.release();
    if (saveVideo) {
        writer.release(); // Make sure to release the writer if we're saving when the loop exits
    }
    return 0;
}
