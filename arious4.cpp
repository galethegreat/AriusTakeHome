#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>

class Point {
private:
    int _index;
    int _value;

public:
    Point(int index, int value) : _index(index), _value(value) {}

    int getIndex() const {
        return _index;
    }

    int getValue() const {
        return _value;
    }

    void setIndex(int index) {
        _index = index;
    }

    void setValue(int value) {
        _value = value;
    }
};

int middleOfSquareWave(int maxIndex, const std::vector<int>& signalData, int errorRange) {
    int p = maxIndex;

    while (p >= 0 && signalData[p] == signalData[maxIndex]) {
        if (maxIndex - p >= (2 * errorRange)) {
            throw std::runtime_error("Square top is too large, larger than permits");
        }

        p--;
    }
    return (maxIndex - p) / 2 + p;
}

Point findMaxPeakIfExistsFromTo(int start, int end, const std::vector<int>& signalData) {
    // Out of bounds
    if (start >= signalData.size() || start < 0 || end >= signalData.size() || end < 0) {
        return Point(-1, std::numeric_limits<int>::min());
    }

    int peakValue = std::numeric_limits<int>::min();
    int peakIndex = -1;
    int p = start;

    while (p <= end) {
        if (signalData[p] > peakValue) {
            peakValue = signalData[p];
            peakIndex = p;
        }
        p++;
    }

    int left = (peakIndex - 1 >= 0) ? signalData[peakIndex - 1] : std::numeric_limits<int>::min();
    int right = (peakIndex < signalData.size() - 1) ? signalData[peakIndex + 1] : std::numeric_limits<int>::min();

    if (left < peakValue && peakValue > right) {
        return Point(peakIndex, peakValue);
    }

    return Point(-1, std::numeric_limits<int>::min());
}

double simpleInterpolation(int leftIndex, int centerIndex, int rightIndex, const std::vector<int>& signalData) {
    double x0 = -1, x1 = 0, x2 = 1;
    double y0 = signalData[leftIndex], y1 = signalData[centerIndex], y2 = signalData[rightIndex];

    double c = y1;
    double b = (y2 - y0) / 2;
    double a = y0 - c + b;

    return (centerIndex + (-b / (2 * a)));
}

double findPeakPosition(const std::vector<int>& signalData) {
    if (signalData.size() < 3) {
        throw std::runtime_error("Signal data is too small to determine the peak");
    }

    const int errorRange = 3;
    Point maxPeakPoint(0, std::numeric_limits<int>::min());

    for (int index = 0; index < signalData.size(); ++index) {
        if (signalData[index] >= maxPeakPoint.getValue()) {
            maxPeakPoint.setIndex(index);
            maxPeakPoint.setValue(signalData[index]);
        }
    }

    if (signalData[maxPeakPoint.getIndex() - 1] == maxPeakPoint.getValue()) {
        return middleOfSquareWave(maxPeakPoint.getIndex(), signalData, errorRange);
    }

    if (maxPeakPoint.getIndex() == 0 || maxPeakPoint.getIndex() == signalData.size() - 1) {
        return maxPeakPoint.getIndex();
    }

    Point leftPeakPoint = findMaxPeakIfExistsFromTo(maxPeakPoint.getIndex() - errorRange, maxPeakPoint.getIndex() - 1, signalData);
    Point rightPeakPoint = findMaxPeakIfExistsFromTo(maxPeakPoint.getIndex() + 1, maxPeakPoint.getIndex() + errorRange, signalData);

    bool noAbnormality = (leftPeakPoint.getIndex() == -1 && rightPeakPoint.getIndex() == -1);
    bool sameValueAbnormality = (leftPeakPoint.getIndex() != -1 && rightPeakPoint.getIndex() != -1 && leftPeakPoint.getValue() == rightPeakPoint.getValue());

    if (noAbnormality) {
        return simpleInterpolation(maxPeakPoint.getIndex() - 1, maxPeakPoint.getIndex(), maxPeakPoint.getIndex() + 1, signalData);
    }

    if (sameValueAbnormality) {
        return maxPeakPoint.getIndex();
    }

    int leftIndex = (leftPeakPoint.getValue() > rightPeakPoint.getValue()) ? leftPeakPoint.getIndex() : maxPeakPoint.getIndex() - 1;
    int rightIndex = (leftPeakPoint.getValue() > rightPeakPoint.getValue()) ? maxPeakPoint.getIndex() + 1 : rightPeakPoint.getIndex();

    return simpleInterpolation(leftIndex, maxPeakPoint.getIndex(), rightIndex, signalData);
}

int main() {
    std::vector<std::vector<int>> data = {
        {1, 2, 3, 4, 5, 6, 5, 4, 3, 2},     // 5
        {1, 2, 3, 4, 5, 10, 9, 4, 3, 2},
        {1, 2, 3, 4, 5, 10, 10, 10, 3, 2},
        {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {10, 7, 9, 8, 6, 5, 4, 3, 2, 1},
        {1, 2, 3, 4, 5, 6, 8, 9, 7, 10},
        {1, 2, 3, 5, 4, 10, 2, 4, 3, 2},
        {1, 2, 3, 4, 5, 10, 7, 9, 5, 2},
        {1, 2, 3, 5, 1, 10, 7, 9, 5, 2},
        {1, 2, 5, 9, 7, 10, 7, 9, 5, 2},
        {1, 2, 5, 9, 7, 10, 2, 3, 1, 0},
        {1, 2, 3, 5, 3, 10, 8, 9, 8, 1},
        {1, 8, 3, 4, 5, 10, 9, 4, 3, 2},
    };

    for (const auto& d : data) {
        try {
            std::cout << findPeakPosition(d) << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
