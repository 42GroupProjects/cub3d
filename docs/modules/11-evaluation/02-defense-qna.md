# 02 — The Brutal Math Defense Q&A

A good evaluator will not just look at your game; they will ask you *why* your game works. If you are using code from a tutorial without understanding it, they will know instantly.

Memorize these answers.

## Q1: Why do we use Perpendicular Distance instead of Euclidean Distance?
**Your Answer:**
"If we use standard Euclidean distance (`sqrt(x^2 + y^2)`), the distance measured to the edges of our FOV will be longer than the distance measured directly in front of the player, even if the wall is perfectly flat. This makes flat walls look curved like a fishbowl. By calculating the perpendicular distance between the ray hit point and the Camera Plane, all points along a flat wall return the exact same distance value, rendering them straight on our screen."

## Q2: How does the DDA Algorithm know whether it hit a vertical or horizontal grid line?
**Your Answer:**
"DDA works by tracking `sideDistX` (distance to the next X grid line) and `sideDistY` (distance to the next Y grid line). Inside the `while` loop, we compare the two. If `sideDistX` is smaller, we jump to the X line, increment `mapX`, and flag `side = 0`. If `sideDistY` is smaller, we jump to the Y line, increment `mapY`, and flag `side = 1`."

## Q3: What is the `deltaDist`?
**Your Answer:**
"It is the exact length a ray must travel along its angle to cross exactly one full grid unit horizontally (`deltaDistX`) or vertically (`deltaDistY`). We calculate it using the absolute value of `1 / rayDir`. It allows DDA to step through grid units efficiently without running heavy `sqrt()` inside the loop."

## Q4: Why don't you use `mlx_pixel_put` directly?
**Your Answer:**
"Because `mlx_pixel_put` makes a direct system call to draw identically to the window immediately. If we call that function 480,000 times a frame (800x600), the overhead drops our game to 1 or 2 FPS. Instead, we use double-buffering: we calculate the entire string of pixels locally in a RAM array, and then push that single massive array to the window once per frame using `mlx_put_image_to_window`."

## Q5: How do you determine the X coordinate for sampling a texture?
**Your Answer:**
"When a ray hits a wall, I find the exact floating-point coordinate of the hit (e.g. `X: 4.3`). I strip the integer part using `floor()` or subtracting the integer value, leaving `0.3`. I then multiply `0.3` by the texture's native width (e.g., 64). `0.3 * 64 = 19.2`. Cast to integer: `19`. Therefore, I need to draw pixel column `19` from the texture."
