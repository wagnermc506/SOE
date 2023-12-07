import time
import re
import os
import cv2
from picamera2 import Picamera2, Preview
from pprint import pprint
from deepface import DeepFace

picam2 = Picamera2()
camera_config = picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)})
picam2.configure(camera_config)
picam2.start()

pattern = re.compile(r'/opt/soe/.*jpg')

while True:
    picam2.capture_file("tmp.jpg")
    
    try:
        result = DeepFace.find(img_path="tmp.jpg", db_path="/opt/soe/db/fotos")
        matches = pattern.search(str(result))
        if matches:
            print('\n{"recognition": "' + str(matches.group()) + '"}\n')
        #result_stripped_list = str(result).split()
        #pprint(result_stripped_list)
        #print('\n{"recognition": "' + result_stripped_list[8]+ '"}\n')
            
    except ValueError:
        pass

    time.sleep(1)

    #image = picam2.capture_array()
    #cv2.imshow("Video", image)
    
    #if cv2.waitKey(1) & 0xFF == ord('q'):
    #    break
    
cv2.destroyAllWindows()
