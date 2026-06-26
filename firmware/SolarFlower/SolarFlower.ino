/* =========================================================================
   SolarFlower.ino
   -------------------------------------------------------------------------
   Auto Solar Pilot — Dual-Axis Autonomous Solar Tracker (ESP32)

   Fully standalone, offline solar tracker. No Wi-Fi, no app, no cloud
   account needed. Four LDR sensors detect the direction of brightest
   sunlight, and two servo motors rotate the panel (horizontal + vertical)
   to follow it automatically — just like a sunflower follows the sun.

   Hardware:
     - ESP32 Dev Board
     - 4 x LDR (Light Dependent Resistor), mounted at the 4 corners of a
       cross-shaped frame on the panel, each with a pull-down resistor
       forming a voltage-divider into an ESP32 ADC pin
     - Servo 1 (horizontal axis) -> GPIO 18
     - Servo 2 (vertical axis)   -> GPIO 19
     - LDR Top    -> GPIO 34 (ADC)
     - LDR Bottom -> GPIO 35 (ADC)
     - LDR Left   -> GPIO 32 (ADC)
     - LDR Right  -> GPIO 33 (ADC)

   How it works:
     1. Read all 4 LDRs.
     2. Compare Left vs Right  -> move horizontal servo toward the brighter side.
     3. Compare Top vs Bottom  -> move vertical servo toward the brighter side.
     4. A "threshold" stops the servo from jittering over tiny light changes.
     5. Repeat forever, with a short delay between checks.
   ========================================================================= */

#include <ESP32Servo.h>

// ---------- Pin definitions ----------
const int LDR_TOP_PIN    = 34;
const int LDR_BOTTOM_PIN = 35;
const int LDR_LEFT_PIN   = 32;
const int LDR_RIGHT_PIN  = 33;

const int SERVO_HORIZONTAL_PIN = 18;
const int SERVO_VERTICAL_PIN   = 19;

// ---------- Tunable settings ----------
// How much difference in light (out of 4095, the ESP32 ADC's max reading)
// is needed before the panel bothers moving. Raise this if the panel jitters.
const int LIGHT_THRESHOLD = 75;

// How many degrees to nudge the servo each time an imbalance is detected.
const int STEP_DEGREES = 1;

// Servo angle safety limits (prevents over-rotation / mechanical strain).
const int SERVO_MIN_ANGLE = 0;
const int SERVO_MAX_ANGLE = 180;

// How long to wait between each sensor check, in milliseconds.
const int LOOP_DELAY_MS = 50;

// ---------- Globals ----------
Servo horizontalServo;
Servo verticalServo;

int horizontalAngle = 90;   // start centered
int verticalAngle   = 90;   // start centered

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n--- Solar Flower: Autonomous LDR Tracker ---");

  // LDR pins are analog inputs - no pinMode needed for ADC on ESP32,
  // but it doesn't hurt to be explicit:
  pinMode(LDR_TOP_PIN, INPUT);
  pinMode(LDR_BOTTOM_PIN, INPUT);
  pinMode(LDR_LEFT_PIN, INPUT);
  pinMode(LDR_RIGHT_PIN, INPUT);

  horizontalServo.attach(SERVO_HORIZONTAL_PIN);
  verticalServo.attach(SERVO_VERTICAL_PIN);

  horizontalServo.write(horizontalAngle);
  verticalServo.write(verticalAngle);

  Serial.println("Setup complete. Starting tracking loop...");
}

void loop() {
  int top    = analogRead(LDR_TOP_PIN);
  int bottom = analogRead(LDR_BOTTOM_PIN);
  int left   = analogRead(LDR_LEFT_PIN);
  int right  = analogRead(LDR_RIGHT_PIN);

  int verticalDiff   = top - bottom;   // positive -> top is brighter
  int horizontalDiff = left - right;   // positive -> left is brighter

  // --- Vertical axis ---
  if (abs(verticalDiff) > LIGHT_THRESHOLD) {
    if (verticalDiff > 0) {
      verticalAngle += STEP_DEGREES;   // top brighter -> tilt up
    } else {
      verticalAngle -= STEP_DEGREES;   // bottom brighter -> tilt down
    }
    verticalAngle = constrain(verticalAngle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    verticalServo.write(verticalAngle);
  }

  // --- Horizontal axis ---
  if (abs(horizontalDiff) > LIGHT_THRESHOLD) {
    if (horizontalDiff > 0) {
      horizontalAngle -= STEP_DEGREES;  // left brighter -> turn left
    } else {
      horizontalAngle += STEP_DEGREES;  // right brighter -> turn right
    }
    horizontalAngle = constrain(horizontalAngle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    horizontalServo.write(horizontalAngle);
  }

  // --- Debug output ---
  Serial.printf(
    "T:%4d B:%4d L:%4d R:%4d  |  H-angle:%3d  V-angle:%3d\n",
    top, bottom, left, right, horizontalAngle, verticalAngle
  );

  delay(LOOP_DELAY_MS);
}
