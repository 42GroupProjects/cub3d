# 02 — Batch converting PNG to XPM

Once you export your 64 animation frames from Blender, they will be `.png` files. MiniLibX requires `.xpm`. Converting 64 files by hand in GIMP will take hours. You need to automate this with your command line shell.

## Option A: ImageMagick (Recommended)
ImageMagick is a free command-line tool standard among graphics developers. 

Install it via homebrew on your mac:
```bash
brew install imagemagick
```

Write a quick bash script inside your `assets/` folder to convert all PNGs at once:
```bash
#!/bin/bash
# convert_all.sh

for file in *.png; do
    # Extract the filename without .png
    filename="${file%.*}"
    
    # Convert preserving transparency
    magick "$file" "$filename.xpm"
    
    echo "Converted $file to $filename.xpm"
done
```

## Option B: FFmpeg
If you do not have ImageMagick, FFmpeg can also handle static image conversions, though it is primarily designed for video.
```bash
# Example conversion for one file
ffmpeg -i sprite_01.png sprite_01.xpm
```

## Cleaning up XPM Colors
Sometimes conversions generate a massive color palette in the XPM header containing thousands of colors (which slows down MLX loading times). 

If your engine takes 5 seconds to boot up after you added your sprites, re-run the conversion and tell ImageMagick to quantize (reduce) the palette to 256 colors maximum:
```bash
magick "$file" -colors 256 "$filename.xpm"
```
