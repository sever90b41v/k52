/// @file  Test suit for Akima spline
/// @brief All tests data for these suite
///        were generated by gnu aspline:
///        $aspline --version
///        aspline version 1.2

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <k52/dsp/akima_spline.h>

namespace
{
// The double and float values in this test suit may not differ more than kTolerance%
const double kTolerance = 0.001;
}

namespace k52
{
namespace dsp
{
namespace tests
{

class AkimaSplineMock : public AkimaSpline
{
public:
    std::size_t ResolveInterval(double value) const
    {
        return AkimaSpline::ResolveInterval(value);
    }
};

BOOST_AUTO_TEST_CASE(ConstructorMustNotThrow)
{
    AkimaSpline::Create();   // Empty Parameters
}

// Akima interpolation require at least 3 points
BOOST_AUTO_TEST_CASE(ResolveIntervalThrowOutOfRange)
{
    double x[] = { -2, -1, 0, 1, 2 }; // it must monotonically increase
    double y[] = {  0,  0, 0, 0, 0 };

    AkimaSplineMock spline;
    spline.Initialize(std::vector<double>(x, x + 5), std::vector<double>(y, y + 5));

    BOOST_REQUIRE_THROW(spline.ResolveInterval(-2.1), std::logic_error);
    BOOST_REQUIRE_THROW(spline.ResolveInterval(2.1), std::logic_error);
}

// Akima interpolation require at least 5 points
BOOST_AUTO_TEST_CASE(InitializeNegativeTrainingPoints)
{
    double x[] = { -0.126543, 0, 1, 2, 3}; // it must monotonically increase
    double y[] = { 0,         0, 0, 0, 0};

    AkimaSplineMock spline;
    BOOST_REQUIRE_THROW(spline.Initialize(std::vector<double>(x, x+2), std::vector<double>(y, y+2)),
                        std::logic_error);

    BOOST_REQUIRE_THROW(spline.Initialize(std::vector<double>(x, x+5), std::vector<double>(y, y+4)),
                        std::logic_error);
}

// Akima calculates values f(x) based on interval x[i] <= x <= x[i+1]
// Current implementation detect interval x[i] <= x < x[i+1]
// so it should correctly detects interval number i based on given value x
BOOST_AUTO_TEST_CASE(CheckResolveInterval)
{
    double x[] = { -0.126543, 0, 0.919, 1.4, 19 }; // it must monotonically increase
    double y[] = { 0,         0, 0,     0,   0 };

    AkimaSplineMock spline;
    spline.Initialize(std::vector<double>(x, x+5), std::vector<double>(y, y+5));

    BOOST_CHECK_EQUAL(spline.ResolveInterval(-0.1), 0);
    BOOST_CHECK_EQUAL(spline.ResolveInterval(0),    1);
    BOOST_CHECK_EQUAL(spline.ResolveInterval(0.8),  1);
    BOOST_CHECK_EQUAL(spline.ResolveInterval(0.92), 2);
    BOOST_CHECK_EQUAL(spline.ResolveInterval(1.5),  3);
    BOOST_CHECK_EQUAL(spline.ResolveInterval(18.9), 3);

    BOOST_CHECK_EQUAL(spline.ResolveInterval(19),   3);
}

// The main Akima spline feature - it doesn't oscillate near interpolated points
BOOST_AUTO_TEST_CASE(AkimaSplineShouldNotOscillate)
{
    double x[] = { -3, -2, -1, 0, 1, 2, 3 }; // it must monotonically increase
    double y[] = {  0,  0,  0, 0, 0, 0, 0 };

    std::vector<double> train_x(x, x+7);
    std::vector<double> train_y(y, y+7);

    AkimaSpline::shared_ptr spline = AkimaSpline::Create();
    spline->Initialize(train_x, train_y);

    // generated for the test points by
    // $aspline --points=10 <test.data>
    BOOST_CHECK_CLOSE(spline->Value(-2.9), 0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-1.9), 0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-0.9), 0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-0.1), 0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(0.1),  0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(0.9),  0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(1.9),  0, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(2.9),  0, kTolerance);
}

// Actually check akima spline
BOOST_AUTO_TEST_CASE(CheckSimpleSet)
{
    double x[] = {  -3,  -2, -1, 0,  1,    2,   3 }; // it must monotonically increase
    double y[] = {-0.1, 0.1,  1, 0, -1, -0.1, 0.1 };

    std::vector<double> train_x(x, x+7);
    std::vector<double> train_y(y, y+7);

    AkimaSpline::shared_ptr spline = AkimaSpline::Create();
    spline->Initialize(train_x, train_y);

    // generated for the test points by
    // $aspline --points=10 <test.data>
    BOOST_CHECK_CLOSE(spline->Value(-3),    -0.1,       kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-2.4),  -0.0407385, kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-2),    0.1,        kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-1.8),  0.275323,   kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-1.2),  1.04683,    kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(-1),    1,          kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(0),     0,          kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(0.6),   -0.6,       kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(1.2),   -1.04683,   kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(1.8),   -0.275323,  kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(2),     -0.1,       kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(2.4),   0.0407385,  kTolerance);
    BOOST_CHECK_CLOSE(spline->Value(3),     0.1,        kTolerance);
}

} // namespace tests
} // namespace dsp
} // namespace k52

