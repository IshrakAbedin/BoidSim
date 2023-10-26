#include "Transformation.h"

namespace Transform
{
    Transform2D Scale2D(double sx, double sy)
    {
        return Transform2D{
            { sx, 0.0, 0.0},
            {0.0,  sy, 0.0},
            {0.0, 0.0, 1.0}
        };
    }

    Transform2D Rotate2D(double theta)
    {
        return Transform2D{
            {blaze::cos(theta), -blaze::sin(theta), 0.0},
            {blaze::sin(theta),  blaze::cos(theta), 0.0},
            {              0.0,                0.0, 1.0}
        };
    }

    Transform2D Translate2D(double dx, double dy)
    {
        return Transform2D{
            {1.0, 0.0,  dx},
            {0.0, 1.0,  dy},
            {0.0, 0.0, 1.0}
        };
    }
}
