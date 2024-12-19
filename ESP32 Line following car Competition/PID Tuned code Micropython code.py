import time
from machine import Pin, PWM

# IR sensor pins
sensor_left = Pin(18, Pin.IN)
sensor_center = Pin(19, Pin.IN)
sensor_right = Pin(25, Pin.IN)

# Motor control pins
left_motor_forward = PWM(Pin(22), freq=1000)
left_motor_backward = PWM(Pin(23), freq=1000)
right_motor_forward = PWM(Pin(32), freq=1000)
right_motor_backward = PWM(Pin(33), freq=1000)

# Speed levels
DEFAULT_SPEED = 600
STOP_SPEED = 0

# PID coefficients (tune these values)
Kp = 1.5  # Proportional gain
Ki = 0.1  # Integral gain
Kd = 0.8  # Derivative gain

# PID variables
previous_error = 0
integral = 0

# Motor control functions
def stop():
    left_motor_forward.duty(int(STOP_SPEED))
    left_motor_backward.duty(int(STOP_SPEED))
    right_motor_forward.duty(int(STOP_SPEED))
    right_motor_backward.duty(int(STOP_SPEED))


def set_motor_speeds(left_speed, right_speed):
    # Ensure speed values are within bounds
    left_speed = max(0, min(1023, int(left_speed)))
    right_speed = max(0, min(1023, int(right_speed)))
    
    left_motor_forward.duty(left_speed)
    right_motor_forward.duty(right_speed)

# Main loop
while True:
    # Read sensor values
    left = sensor_left.value()
    center = sensor_center.value()
    right = sensor_right.value()

    # Determine the error based on sensor readings
    if left == 1 and center == 0 and right == 0:
        error = -1  # Line is on the left
    elif right == 1 and center == 0 and left == 0:
        error = 1   # Line is on the right
    elif center == 1:
        error = 0   # Line is centered
    else:
        error = 0   # No line detected (default to no error)

    # PID calculations
    integral += error  # Accumulate the integral
    derivative = error - previous_error  # Calculate the derivative
    output = Kp * error + Ki * integral + Kd * derivative  # Compute PID output
    previous_error = error  # Update previous error for next iteration

    # Adjust motor speeds based on PID output
    left_speed = DEFAULT_SPEED - output
    right_speed = DEFAULT_SPEED + output

    set_motor_speeds(left_speed, right_speed)

    # Debugging output
    print(f"Left: {left}, Center: {center}, Right: {right}, Error: {error}, Output: {output}")
    
    time.sleep(0.05)  # Small delay for loop stability
