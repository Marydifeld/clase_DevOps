#include <gtest/gtest.h>
#include "grade_calculator.h"
#include "../../include/student_service.h"
#include <stdexcept>

TEST(GradeCalculatorTest, AverageShouldReturnCorrectValue) {
    const std::vector<double> grades{80, 90, 100};
    EXPECT_DOUBLE_EQ(GradeCalculator::average(grades), 90.0);
}

TEST(GradeCalculatorTest, PassedShouldReturnTrueWhenAverageIsEnough) {
    const std::vector<double> grades{70, 75, 80};
    EXPECT_TRUE(GradeCalculator::passed(grades));
}

TEST(GradeCalculatorTest, PassedShouldReturnFalseWhenAverageIsLow) {
    const std::vector<double> grades{50, 60, 65};
    EXPECT_FALSE(GradeCalculator::passed(grades));
}

TEST(GradeCalculatorTest, AverageShouldThrowOnEmptyInput) {
    const std::vector<double> grades{};
    EXPECT_THROW(GradeCalculator::average(grades), std::invalid_argument);
}

TEST(GradeCalculatorTest, AverageShouldThrowOnInvalidGrade) {
    const std::vector<double> grades{90, 110};
    EXPECT_THROW(GradeCalculator::average(grades), std::invalid_argument);
}

TEST(StudentServiceTest, HealthReturnsUp) {
    std::string res = StudentService::health();
    EXPECT_TRUE(res.find("UP") != std::string::npos);
}

TEST(StudentServiceTest, CalculateFinalGradeSuccess) {
    std::string body = "{\"grades\": [90, 80, 70]}";
    std::string res = StudentService::calculateFinalGradeJson(body);
    EXPECT_TRUE(res.find("80.0") != std::string::npos);
    EXPECT_TRUE(res.find("true") != std::string::npos);
}

TEST(StudentServiceTest, CalculateFinalGradeThrowsOnInvalidJson) {
    std::string body = "{\"wrong_field\": 123}";
    EXPECT_THROW(StudentService::calculateFinalGradeJson(body), std::invalid_argument);
}