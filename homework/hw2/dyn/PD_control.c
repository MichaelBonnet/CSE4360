#include <stdio.h>
#include <math.h>

double prev_theta_dot = 0;
double delta_time = 0.002;

double PD_control(double theta, double theta_dot, double theta_ref, double theta_dot_ref)
{
    // theta         = current joint angle
    // theta_dot     = corresponding rotational velocity
    // theta_ref     = desired reference angle, set using the slider
    // theta_dot_ref = reference velocity

    // τ = I*Θdotdot + B*Θdot + G*(Θ)
    // τ -> torque
    // I -> inertia of the arm
    // B -> viscous friction in the joint
    // G -> gravity
    // Θdotdot -> difference between last and current thetas ("delta theta")
    // Θdot    -> corresponding rotational velocity
    // Θ       -> current joint angle

    /* ============== *
     * === G Term === *
     * ============== */

    // Math behind it:

    // τ = I*Θdotdot + B*Θdot + G*(Θ)
    // 1 = 0 + 0 + G*(Θ)
    // 1 = G*(Θ)
    // 1 = mg * cos(Θ)
    // mg = 1/cos(Θ)

    // For testing and finding out mgl
    // printf("mg = %f\n", 1/cos(theta));

    double mg     = 1.1760;
    double G_term = mg * cos(theta);

    /* ============== *
     * === B Term === *
     * ============== */

    // Math behind it:

    // τ = I*Θdotdot + B*Θdot + G*(Θ)
    // τ = τ_prime + G*(Θ)
    // τ_prime = I*Θdotdot + B*Θdot
    // 1 = (0) + B*Θdot // I term is acceleration, and with B*theta_dot velocity is constant
    // B = 1/Θdot

    // For testing and finding out B
    // printf("B = %f\n", 1/theta_dot);
    double B_friction = 0.1000;
    double B_term     = B_friction * theta_dot;

    /* ============== *
     * === I Term === *
     * ============== */

    // Math behind it:

    // τ = I*Θdotdot + B*Θdot + G*(Θ)
    // τ = 1 + B*Θdot + G*(Θ)
    // 1 = I*Θdotdot
    // I = 1/Θdotdot
    // Θdotdot = delta_theta / delta_time
    // Θdotdot = (theta_dot - prev_theta_dot) / (1 / freq)
    // Θdotdot = (theta_dot - prev_theta_dot) / (1 / 500 hz)

    double theta_double_dot = (theta_dot - prev_theta_dot) / delta_time;
    prev_theta_dot = theta_dot; // update the prev_theta_dot global var

    // For testing and finding out I
    // printf("I = %f\n", 1/theta_double_dot);
    double I_inertia = 0.0360;
    double I_term    = I_inertia * theta_double_dot;

    /* ============== *
     * === τ Term === *
     * ============== */

    // Math behind it:

    // τ = I*Θdotdot + B*Θdot + G*(Θ)

    // Total term, having found out each of the 3 component terms
    double torque = I_term + B_term + G_term;

    /* ============== *
     * === PART 5 === *
     * ============== */

    // Might do later

    // Return it
    return(torque);
}
