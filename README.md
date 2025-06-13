# esp-startrek-tng-ornament

A real-time animated Star Trek TNG-style ornament for ESP32 and 240x240 TFT displays. It draws a smooth parallax starfield, centered around a BMP image of the USS Enterprise, with optional alternate ships like the Borg cube. Designed for round displays but works fine on square ones too.

This is a fully self-contained visual — no buttons, no WiFi, just plug it in and enjoy the Trek vibes.
⚙️ Setup Instructions

    Use Arduino IDE 2.0.17+ (or PlatformIO)
    This was built using the ESP32 core v2.0.17, which plays nicely with SPIFFS and TFTs.

    Install Libraries:

        TFT_eSPI — you must configure it correctly for your screen in User_Setup.h

        SPIFFS — already included in the ESP32 board package

    Configure User_Setup.h in TFT_eSPI
    Use your own screen setup. No specific driver is hardcoded.
    Example (if you need one):

#define ILI9341_DRIVER
#define TFT_WIDTH 240
#define TFT_HEIGHT 240
#define TFT_CS    5
#define TFT_DC    16
#define TFT_RST   23

But again — just use whatever works with your display.

SPIFFS Upload:
This repo includes a /data folder with:

    enterprise.bmp

    borg.bmp

To upload it:

    In Arduino IDE, use the SPIFFS upload tool

    In PlatformIO:

        pio run --target uploadfs

    Run the sketch
    The stars animate at ~60 FPS, dodging a static image of the Enterprise.
    You can change the image easily or even make it cycle between ships.

✨ Features

    Parallax animated starfield (80 stars with 3 depth levels)

    BMP image rendering from SPIFFS

    Includes both Enterprise and Borg images by default

    Stars avoid the ship with a smart dead zone

    Round display compatible (looks great in 240x240)

    No buttons, no inputs, just visuals

🧠 Image Customization

You can swap in your own BMPs:

    Must be 24-bit uncompressed BMP

    Around 50×50 px is a good size for performance

    Add them to the /data/ folder and upload via SPIFFS

    In code, draw like this:

    displayBitmap("/borg.bmp", 80, 80);

You can also randomize which ship appears or alternate between them — just ask if you want help adding that!
📁 File Layout

esp-startrek-tng-ornament/
├── data/
│   ├── enterprise.bmp
│   └── borg.bmp
├── src/
│   └── main.ino
└── platformio.ini  (optional)

🧼 Notes

    This was thrown together for display fun — if anything's messy, tweak away

    If the BMPs don't show, make sure they're 24-bit and uploaded to SPIFFS

    Looks best on round screens, but still works on any 240x240 TFT

Let me know if you'd like a version that:

    cycles between the Enterprise and Borg every few seconds

    adds some warp effects or blinking stars

    uses a low-power display mode for real ornament usage

I'd be happy to help.
