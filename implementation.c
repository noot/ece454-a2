#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveDownReference(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    memmove(rendered_frame, buffer_frame+offset*width*3, width*3*height - width*3*offset);
    memset(rendered_frame + (height-offset)*width*3, 255, offset*width*3);
    memmove(buffer_frame, rendered_frame, sizeof(rendered_frame));

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveLeftReference(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    // for (int row = 0; row < height; row++) {
    //     for (int column = offset; column < width; column++) {
    //         int position_rendered_frame = row * width * 3 + column * 3;
    //         int position_buffer_frame = row * width * 3 + (column - offset) * 3;
    //         rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
    //         rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
    //         rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
    //     }
    // }

    // // fill left over pixels with white pixels
    // for (int row = 0; row < height; row++) {
    //     for (int column = 0; column < offset; column++) {
    //         int position_rendered_frame = row * width * 3 + column * 3;
    //         rendered_frame[position_rendered_frame] = 255;
    //         rendered_frame[position_rendered_frame + 1] = 255;
    //         rendered_frame[position_rendered_frame + 2] = 255;
    //     }
    // }

    memmove(rendered_frame, buffer_frame+offset*height*3, height*3*width - height*3*offset);
    memset(rendered_frame + (width-offset)*width*3, 255, offset*width*3);
    memmove(buffer_frame, rendered_frame, sizeof(rendered_frame));

    // copy the temporary buffer back to original frame buffer
    //buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveUpReference(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // // store shifted pixels to temporary buffer
    // for (int row = offset; row < height; row++) {
    //     for (int column = 0; column < width; column++) {
    //         int position_rendered_frame = row * width * 3 + column * 3;
    //         int position_buffer_frame = (row - offset) * width * 3 + column * 3;
    //         rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
    //         rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
    //         rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
    //     }
    // }

    // // fill left over pixels with white pixels
    // for (int row = 0; row < offset; row++) {
    //     for (int column = 0; column < width; column++) {
    //         int position_rendered_frame = row * width * 3 + column * 3;
    //         rendered_frame[position_rendered_frame] = 255;
    //         rendered_frame[position_rendered_frame + 1] = 255;
    //         rendered_frame[position_rendered_frame + 2] = 255;
    //     }
    // }

    // // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    memmove(buffer_frame+offset*width*3, buffer_frame, width*3*(height-offset));
    // memset(rendered_frame + (height-offset)*width*3, 255, offset*width*3);
    // memmove(buffer_frame, rendered_frame, sizeof(rendered_frame));

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveRightReference(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    // for (int row = 0; row < height; row++) {
    //     for (int column = 0; column < (width - offset); column++) {
    //         int position_rendered_frame = row * width * 3 + column * 3;
    //         int position_buffer_frame = row * width * 3 + (column + offset) * 3;
    //         rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
    //         rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
    //         rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
    //     }
    // }

    // // fill left over pixels with white pixels
    // for (int row = 0; row < height; row++) {
    //     for (int column = width - offset; column < width; column++) {
    //         int position_rendered_frame = row * width * 3 + column * 3;
    //         rendered_frame[position_rendered_frame] = 255;
    //         rendered_frame[position_rendered_frame + 1] = 255;
    //         rendered_frame[position_rendered_frame + 2] = 255;
    //     }
    // }

    // // copy the temporary buffer back to original frame buffer
    // buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    memmove(rendered_frame+offset*height*3, buffer_frame, height*3*(width - offset));
    memset(rendered_frame + (width-offset)*width*3, 255, offset*width*3);
    memmove(buffer_frame, rendered_frame, sizeof(rendered_frame));

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                               int rotate_iteration) {
    return processRotateCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                int rotate_iteration) {
    return processRotateCCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {
    return processMirrorXReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
    return processMirrorYReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "noot";

    // Please fill in your information
    char student_first_name[] = "Elizabeth";
    char student_last_name[] = "Binks";
    char student_student_number[] = "1001274676";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent_first_name: %s\n", student_first_name);
    printf("\tstudent_last_name: %s\n", student_last_name);
    printf("\tstudent_student_number: %s\n", student_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int processed_frames = 0;
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
            frame_buffer = processMoveUp(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
            frame_buffer = processMoveLeft(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
            frame_buffer = processMoveDown(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
            frame_buffer = processMoveRight(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
            frame_buffer = processRotateCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
            frame_buffer = processRotateCCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            frame_buffer = processMirrorX(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            frame_buffer = processMirrorY(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            verifyFrame(frame_buffer, width, height, grading_mode);
        }
    }
    return;
}
