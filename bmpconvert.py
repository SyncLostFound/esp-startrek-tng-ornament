from PIL import Image
import os

def convert_32bmp_to_24bmp(input_path, output_path):
    # Open the image using PIL
    with Image.open(input_path) as img:
        # Ensure the image is in 'RGB' mode (24-bit)
        img = img.convert("RGB")
        # Save the image as a 24-bit BMP
        img.save(output_path, format='BMP')

    print(f"Conversion complete: {output_path}")

# Replace 'input_image.bmp' with the path to your 32-bit BMP file
input_image = 'YOUR IMAGE'
# Replace 'output_image.bmp' with the desired output path for the 24-bit BMP file
output_image = 'OUTPUT DIR'

# Convert the image
convert_32bmp_to_24bmp(input_image, output_image)
