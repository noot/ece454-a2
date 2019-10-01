#include <stdio.h>
#include <stdlib.h>
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

    //return processMoveUpReference(buffer_frame, width, height, offset);

    memmove(buffer_frame, buffer_frame+offset*width*3, width*3*height - width*3*offset);

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

    //return processMoveRightReference(buffer_frame, width, height, offset);

    // for each row, shift right by offset
    for (int row = 0; row < height; row++) {
        memmove(buffer_frame + row*width*3 + offset*3, buffer_frame+row*width*3, 3*(width-offset));
        memset(buffer_frame + row*width*3, 255, offset*3);
    }

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

    //return processMoveDownReference(buffer_frame, width, height, offset);

    memmove(buffer_frame+offset*width*3, buffer_frame, width*3*(height-offset));

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

    //return processMoveLeftReference(buffer_frame, width, height, offset);

    // for each row, shift right by offset
    for (int row = 0; row < height; row++) {
        memmove(buffer_frame + row*width*3, buffer_frame+row*width*3  + offset*3, 3*(width-offset));
        memset(buffer_frame + (row+1)*width*3 - offset*3, 255, offset*3);
    }

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
    // handle negative offsets
    if (rotate_iteration < 0){
        return processRotateCCWReference(buffer_frame, width, height, rotate_iteration * -1);
    }

    if (rotate_iteration%4 == 1) {
        rotate_iteration = 1;
    } else if (rotate_iteration%4 == 2) {
        rotate_iteration = 2;
    } else if (rotate_iteration%4 == 3) {
        rotate_iteration = 3;
    } else {
        return buffer_frame;
    }

    int render_col = width-1;
    int render_row = 0;
    int width_pixels = width*3;
    unsigned char *tmp_pixel = (unsigned char*)malloc(3*sizeof(unsigned char));

    switch(rotate_iteration) {
        case 1:
            for (int row = 0; row < width/2; row++) {
                for (int column = 0; column < height/2; column++) {
                    int render_pos_a = render_row*width_pixels + render_col*3;
                    int render_pos_b = (height-row-1)*width_pixels + (width-column-1)*3;
                    int render_pos_c = (height-render_row-1)*width_pixels + (width-render_col-1)*3;
                    int original_pos = row*width_pixels + column*3;

                    // save original pixel to temp
                    memcpy(tmp_pixel, buffer_frame + original_pos, 3);
                    // copy pixel c to original pixel
                    memcpy(buffer_frame + original_pos, buffer_frame + render_pos_c, 3);
                    // copy pixel b to pixel c
                    memcpy(buffer_frame + render_pos_c, buffer_frame + render_pos_b, 3);
                    // // copy pixel a to pixel b
                    memcpy(buffer_frame + render_pos_b, buffer_frame + render_pos_a, 3);
                    // // copy temp to pixel a
                    memcpy(buffer_frame + render_pos_a, tmp_pixel, 3);
                    render_row++;
                }
                render_row = 0;
                render_col--;
            }
            break;
        case 2:
            for (int row = 0; row < width/2; row++) {
                for (int column = 0; column < height; column++) {
                    int render_pos = (height-row-1)*width_pixels + (width-column-1)*3;
                    int original_pos = row*width_pixels + column*3;
                    memcpy(tmp_pixel, buffer_frame + original_pos, 3);
                    memcpy(buffer_frame + original_pos, buffer_frame + render_pos, 3);
                    memcpy(buffer_frame + render_pos, tmp_pixel, 3);
                }
            }
            break;
        case 3:
            for (int row = 0; row < width/2; row++) {
                for (int column = 0; column < height/2; column++) {
                    int render_pos_a = render_row*width_pixels + render_col*3;
                    int render_pos_b = (height-row-1)*width_pixels + (width-column-1)*3;
                    int render_pos_c = (height-render_row-1)*width_pixels + (width-render_col-1)*3;
                    int original_pos = row*width_pixels + column*3;

                    // save original pixel to temp
                    memcpy(tmp_pixel, buffer_frame + original_pos, 3);
                    // copy pixel a to original pixel
                    memcpy(buffer_frame + original_pos, buffer_frame + render_pos_a, 3);
                    // copy pixel b to pixel a
                    memcpy(buffer_frame + render_pos_a, buffer_frame + render_pos_b, 3);
                    // // copy pixel c to pixel b
                    memcpy(buffer_frame + render_pos_b, buffer_frame + render_pos_c, 3);
                    // // copy temp to pixel c
                    memcpy(buffer_frame + render_pos_c, tmp_pixel, 3);
                    render_row++;
                }
                render_row = 0;
                render_col--;
            }
            break;
        default:
            break;
    }

    free(tmp_pixel);

    // return a pointer to the updated image buffer
    return buffer_frame;
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
    if (rotate_iteration < 0){
        // handle negative offsets
        // rotating 90 degrees counter clockwise in opposite direction is equal to 90 degrees in cw direction
        buffer_frame = processRotateCW(buffer_frame, width, height, -1 * rotate_iteration);
    } else {
        if (rotate_iteration%4 == 1) {
            rotate_iteration = 3;
        } else if (rotate_iteration%4 == 2) {
            rotate_iteration = 2;
        } else if (rotate_iteration%4 == 3) {
            rotate_iteration = 1;
        } else {
            return buffer_frame;
        }

        // rotating 90 degrees counter clockwise is equivalent of rotating 270 degrees clockwise
        buffer_frame = processRotateCW(buffer_frame, width, height, rotate_iteration);
    }

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {
    unsigned char *temp_row = (unsigned char*)malloc(width*3*sizeof(char));
    int width_pixels = width*3;

    for (int row = 0; row < height/2; row++) {
        // copy row from top half of frame to temp
        memcpy(temp_row, buffer_frame+row*width_pixels, width_pixels);
        // copy row from bottom half to top half of frame
        memcpy(buffer_frame+row*width_pixels, buffer_frame+(height-row-1)*width_pixels, width_pixels);
        // copy row that was saved from top half to bottom half
        memcpy(buffer_frame+(height-row-1)*width_pixels, temp_row, width_pixels);
    }

    free(temp_row);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
    unsigned char *temp_row = (unsigned char*)malloc((width/2)*3*sizeof(char));
    int width_pixels = width*3;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width/2; col++) {
            int right_pos = row*width_pixels + (width-col-1)*3;
            int left_pos = row*width_pixels + col*3;
            // copy right side of row into temp in reverse order
            memcpy(temp_row + col, buffer_frame + right_pos, 3);
            // copy left side of buffer row into right side of buffer row in reverse order
            memcpy(buffer_frame + right_pos, buffer_frame + left_pos, 3);
            // copy temp pixel back into left side of buffer
            memcpy(buffer_frame + left_pos, temp_row + col, 3);
        }
         // copy temp half-row which contains new left side back into left half of buffer
        //memcpy(buffer_frame + row*width*3, temp_row, width*3/2);
    }

    free(temp_row);

    // return a pointer to the updated image buffer
    return buffer_frame;
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
