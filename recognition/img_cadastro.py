from picamera2 import Picamera2, Preview
from pprint import pprint
import cv2

picam2 = Picamera2()
camera_config = picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (640*2, 480*2)})
picam2.configure(camera_config)
picam2.start()

while True:
    image = picam2.capture_array()
    #image = picam2.capture_file("tmp.jpg")

    cv2.imshow("Video", image)
    
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
    elif key == ord('c'):
        picam2.capture_file("tmp2.jpg")
        print("file tmp2.jpg saved")
    
    
cv2.destroyAllWindows()

