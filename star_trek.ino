#include <SPI.h>
#include <TFT_eSPI.h>
#include <SPIFFS.h>

// Define the screen dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

// Custom color definitions
#define TFT_GREY 0x7BEF       // Medium grey
#define TFT_LIGHTGREY 0xC618  // Light grey

// Create a display object
TFT_eSPI display = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);

// Starfield configuration
int starField[80][3]; // Array for 80 stars with x, y positions and depth
unsigned long lastStarUpdate = 0;

// Enterprise position and dead zone dimensions
int enterpriseX = 60; // X position of the Enterprise
int enterpriseY = 60; // Y position of the Enterprise
int enterpriseWidth = 50; // Width of the Enterprise BMP
int enterpriseHeight = 50; // Height of the Enterprise BMP
int deadZoneMargin = 10; // Additional margin around the Enterprise for the dead zone

void setup() {
  Serial.begin(115200);
  display.begin();
  display.setRotation(2);
  display.fillScreen(TFT_BLACK);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Draw the USS Enterprise BMP once
  drawEnterprise();

  // Initialize star positions with depth, covering the entire screen
  for (int i = 0; i < 80; i++) {
    // Randomize positions outside the dead zone
    do {
      starField[i][0] = random(0, SCREEN_WIDTH);
      starField[i][1] = random(0, SCREEN_HEIGHT);
    } while (isInDeadZone(starField[i][0], starField[i][1]));
    
    starField[i][2] = random(1, 4); // Depth from 1 (closest) to 3 (farthest)
  }
}

void loop() {
  // Refresh starfield
  if (millis() - lastStarUpdate > 16) {
    drawParallaxStarField();
    lastStarUpdate = millis();
  }
}

void drawParallaxStarField() {
  // Draw the starfield directly on the screen for simplicity
  for (int i = 0; i < 80; i++) {
    // Erase the previous star position by drawing over it with black
    display.drawPixel(starField[i][0], starField[i][1], TFT_BLACK);

    // Update star position based on its depth for diagonal movement
    int starSpeed = starField[i][2]; // Speed increases with lower depth values (closer stars)
    starField[i][0] += starSpeed; // Move right
    starField[i][1] += starSpeed; // Move down

    // Check if the star is in the dead zone
    if (isInDeadZone(starField[i][0], starField[i][1])) {
      starField[i][0] = random(0, SCREEN_WIDTH);
      starField[i][1] = random(0, SCREEN_HEIGHT);
    }

    // Wrap the star back to the top-left if it moves off-screen
    if (starField[i][0] >= SCREEN_WIDTH || starField[i][1] >= SCREEN_HEIGHT) {
      do {
        starField[i][0] = random(0, SCREEN_WIDTH);
        starField[i][1] = random(0, SCREEN_HEIGHT);
      } while (isInDeadZone(starField[i][0], starField[i][1]));
      starField[i][2] = random(1, 4); // parallax effect
    }

    // Draw new star with brightness based on depth
    uint16_t starColor = (starField[i][2] == 1) ? TFT_WHITE : 
                         (starField[i][2] == 2) ? TFT_LIGHTGREY : 
                         TFT_GREY;
    if (!isInDeadZone(starField[i][0], starField[i][1])) {
      display.drawPixel(starField[i][0], starField[i][1], starColor);
    }
  }
}

void drawEnterprise() {
  // Draw the USS Enterprise BMP in a fixed position
  displayBitmap("/enterprise.bmp", enterpriseX, enterpriseY);
}

bool isInDeadZone(int x, int y) {
  // Define the dead zone around the Enterprise, with a margin
  int xMin = enterpriseX - deadZoneMargin;
  int xMax = enterpriseX + enterpriseWidth + deadZoneMargin;
  int yMin = enterpriseY - deadZoneMargin;
  int yMax = enterpriseY + enterpriseHeight + deadZoneMargin;
  return (x >= xMin && x <= xMax && y >= yMin && y <= yMax);
}

void displayBitmap(const char *filename, int16_t x, int16_t y) {
  fs::File bmpFile = SPIFFS.open(filename, "r");
  if (!bmpFile) {
    Serial.print("File not found: ");
    Serial.println(filename);
    return;
  }

  uint8_t header[54];
  bmpFile.read(header, 54); // Read BMP header

  int16_t width = header[18] | (header[19] << 8);
  int16_t height = header[22] | (header[23] << 8);

  // Read pixel data and draw it on the display
  for (int16_t row = height - 1; row >= 0; row--) {
    for (int16_t col = 0; col < width; col++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();
      uint16_t color = display.color565(r, g, b);
      display.drawPixel(x + col, y + row, color);
    }
  }

  bmpFile.close();
}
