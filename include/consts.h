const int M3_B = 31;
const int M3_A = 29;
const int M4_B = 27;
const int M4_A = 25;

const int ENCODER_PINS[] = {
    M3_B, // RIGHT
    M3_A, // RIGHT
    M4_B, // LEFT
    M4_A, // LEFT
};

const int MOTOR_PINS[] = {
    3, 5, // M1 -- RIGHT
    6, 8, // M2 -- LEFT
    7, 9, // M3 -- RIGHT
    2, 4, // M4 -- LEFT
};

int const start_motor_speed = 80;