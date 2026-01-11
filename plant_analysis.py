from flask import Flask, request
import cv2
import numpy as np
from colorthief import ColorThief
from io import BytesIO

app = Flask(__name__)



def analyze_plant_color(image_bytes):
    try:
        color_thief = ColorThief(BytesIO(image_bytes))
        dominant_color = color_thief.get_color(quality=1)
        r, g, b = dominant_color

        img_np = np.frombuffer(image_bytes, np.uint8)
        img = cv2.imdecode(img_np, cv2.IMREAD_COLOR)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        lower_green = np.array([36, 60, 40])
        upper_green = np.array([89, 255, 255])
        lower_yellow = np.array([20, 100, 100])
        upper_yellow = np.array([35, 255, 255])
        lower_brown = np.array([10, 80, 20])
        upper_brown = np.array([20, 255, 180])

        mask_green = cv2.inRange(hsv, lower_green, upper_green)
        mask_yellow = cv2.inRange(hsv, lower_yellow, upper_yellow)
        mask_brown = cv2.inRange(hsv, lower_brown, upper_brown)

        total_pixels = mask_green.size
        green_percent = cv2.countNonZero(mask_green) / total_pixels * 100
        yellow_percent = cv2.countNonZero(mask_yellow) / total_pixels * 100
        brown_percent = cv2.countNonZero(mask_brown) / total_pixels * 100

        print(f"Green: {green_percent:.2f}% | Yellow: {yellow_percent:.2f}% | Brown: {brown_percent:.2f}%")

        if green_percent >= yellow_percent and green_percent >= brown_percent:
            if green_percent >= 35:
                return "Healthy plant (Green)", dominant_color
            else:
                return "Healthy plant (Dominant Green)", dominant_color
        elif yellow_percent >= green_percent and yellow_percent >= brown_percent:
            if yellow_percent > 15:
                return "Undernourished plant (Yellow)", dominant_color
            else:
                return "Undernourished plant (Dominant Yellow)", dominant_color
        elif brown_percent >= green_percent and brown_percent >= yellow_percent:
            if brown_percent > 12:
                return "Dead plant (Brown)", dominant_color
            else:
                return "Dead plant (Dominant Brown)", dominant_color

        if r > 180 and g > 160 and b < 100:
            return "Undernourished plant (Yellow RGB)", dominant_color
        elif r > 140 and g < 100 and b < 80:
            return "Dead plant (Brown RGB)", dominant_color
        elif g > 120 and r < 100:
            return "Healthy plant (Green RGB)", dominant_color

        return "Moderately healthy plant", dominant_color

    except Exception as e:
        return f"Error during analysis: {str(e)}", (0, 0, 0)

@app.route('/analyze', methods=['POST'])
def analyze():
    if not request.data:
        return "Error: No image data", 400

    try:
        result, color = analyze_plant_color(request.data)
        print(f"Analysis: {result} | Dominant RGB: {color}")
        # Store result for status check
        global last_status
        last_status = result
        return result
    except Exception as e:
        print(f"Error: {str(e)}")
        return f"Error: {str(e)}", 500

# Global variable to hold last analysis
last_status = "Unknown"

@app.route('/status', methods=['GET'])
def status():
    return last_status

# ---------------------------
# Run Flask app last
# ---------------------------
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)


