#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveMirrorX(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);

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
        memmove(buffer_frame+offset*width*3, buffer_frame, width*3*(height-offset));
        return buffer_frame;
    }

    memmove(buffer_frame, buffer_frame+offset*width*3, width*3*height - width*3*offset);
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
    if (offset < 0) {
        // for each row, shift right by offset
        for (int row = 0; row < height; row++) {
            memmove(buffer_frame + row*width*3, buffer_frame+row*width*3  + offset*3, 3*(width-offset));
            memset(buffer_frame + (row+1)*width*3 - offset*3, 255, offset*3);
        }
        return buffer_frame;
        //return processMoveLeft(buffer_frame, width, height, offset * -1);
    }

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
        memmove(buffer_frame, buffer_frame+offset*width*3, width*3*height - width*3*offset);
        return buffer_frame;
    }

    memmove(buffer_frame+offset*width*3, buffer_frame, width*3*(height-offset));
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
        // for each row, shift right by offset
        for (int row = 0; row < height; row++) {
            memmove(buffer_frame + row*width*3 + offset*3, buffer_frame+row*width*3, 3*(width-offset));
            memset(buffer_frame + row*width*3, 255, offset*3);
        }
    }

    // for each row, shift right by offset
    for (int row = 0; row < height; row++) {
        memmove(buffer_frame + row*width*3, buffer_frame+row*width*3  + offset*3, 3*(width-offset));
        memset(buffer_frame + (row+1)*width*3 - offset*3, 255, offset*3);
    }

    // return a pointer to the updated image buffer
    return buffer_frame;
}

unsigned char *rotateSubsquareCW(unsigned char *buffer_frame, unsigned int width) {
    unsigned char *tmp_pixel = (unsigned char*)malloc(3*sizeof(unsigned char));
    int render_col = width-1;
    int render_row = 0;
    int width_pixels = width*3;
    int height = width;

    unsigned char *white_pixel = (unsigned char *)malloc(3*sizeof(unsigned char));
    memset(white_pixel, 255, 3);

    for (int row = 0; row < width>>1; row++) {
        for (int column = 0; column < height>>1; column++) {
            int render_pos_a = render_row*width_pixels + render_col*3;
            int render_pos_b = (height-row-1)*width_pixels + (width-column-1)*3;
            int render_pos_c = (height-render_row-1)*width_pixels + (width-render_col-1)*3;
            int original_pos = row*width_pixels + column*3;

            if (memcmp(buffer_frame+original_pos, white_pixel, 3) != 0 ||
                memcmp(buffer_frame+render_pos_c, white_pixel, 3) != 0 ||
                memcmp(buffer_frame+render_pos_b, white_pixel, 3) != 0 ||
                memcmp(buffer_frame+render_pos_a, white_pixel, 3) != 0) {
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
            }
            render_row++;
        }
        render_row = 0;
        render_col--;
    }  

    free(tmp_pixel);
    free(white_pixel);
    return buffer_frame;
}

unsigned char *rotateSubsquareCCW(unsigned char *buffer_frame, unsigned int width) {
    unsigned char *tmp_pixel = (unsigned char*)malloc(3*sizeof(unsigned char));
    int render_col = width-1;
    int render_row = 0;
    int width_pixels = width*3;
    int height = width;
    unsigned char *white_pixel = (unsigned char *)malloc(3*sizeof(unsigned char));
    memset(white_pixel, 255, 3);

    for (int row = 0; row < width>>1; row++) {
        for (int column = 0; column < height>>1; column++) {
            int render_pos_a = render_row*width_pixels + render_col*3;
            int render_pos_b = (height-row-1)*width_pixels + (width-column-1)*3;
            int render_pos_c = (height-render_row-1)*width_pixels + (width-render_col-1)*3;
            int original_pos = row*width_pixels + column*3;

            if (memcmp(buffer_frame+original_pos, white_pixel, 3) != 0 ||
                memcmp(buffer_frame+render_pos_c, white_pixel, 3) != 0 ||
                memcmp(buffer_frame+render_pos_b, white_pixel, 3) != 0 ||
                memcmp(buffer_frame+render_pos_a, white_pixel, 3) != 0) {

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
            }
            render_row++;
        }
        render_row = 0;
        render_col--;
    }

    free(tmp_pixel);
    free(white_pixel);
    return buffer_frame;
}

unsigned char *rotateSubsquare180(unsigned char *buffer_frame, unsigned int width) {
    unsigned char *tmp_pixel = (unsigned char*)malloc(3*sizeof(unsigned char));
    int width_pixels = width*3;
    int height = width;

    for (int row = 0; row < width>>1; row++) {
        for (int column = 0; column < height; column++) {
            int render_pos = (height-row-1)*width_pixels + (width-column-1)*3;
            int original_pos = row*width_pixels + column*3;
            memcpy(tmp_pixel, buffer_frame + original_pos, 3);
            memcpy(buffer_frame + original_pos, buffer_frame + render_pos, 3);
            memcpy(buffer_frame + render_pos, tmp_pixel, 3);
        }
    }

    free(tmp_pixel);
    return buffer_frame;
}

bool isSubsquareWhite(unsigned char *buffer_frame, unsigned width) {
    unsigned char* whitesubsquare = (unsigned char*)malloc(width*width*3*sizeof(unsigned char));
    memset(whitesubsquare, 255, width*width*3);
    if (memcmp(whitesubsquare, buffer_frame, width*width*3) != 0) return false;
    return true;
}

// num_divisions is how many times the rows/columns get divided
// total # of subsquares = num_divisions*num_divisions
// subsquares array is arranged in row-order
// eg. buffer_frame: 
//  ___ ___ ___
// |_0_|_1_|_2_|
// |_4_|_5_|_6_|
// |_7_|_8_|_9_|
// 
// num_divisions = 3; subsquares: | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 
unsigned char **divideIntoSubsquares(unsigned char *buffer_frame, unsigned width, unsigned num_divisions) {
    unsigned char **subsquares = (unsigned char**)malloc(num_divisions*num_divisions*sizeof(unsigned char*));
    int subsquare_width = width/num_divisions;
    // int excess = width%num_divisions;
    // int middle_index = num_divisions/2;

    // let's add excess pixels to the middle subsquare to make things easier for rotation

    for (int i = 0; i < num_divisions*num_divisions; i++) {
        subsquares[i] = (unsigned char*)malloc(subsquare_width*subsquare_width*3*sizeof(unsigned char));
        unsigned char *ss = subsquares[i];
        int column = (i%num_divisions) * subsquare_width;
        // copy row by row into subsquare
        for (int row = 0; row < subsquare_width; row++) {
            int row_within_buffer_frame = i*subsquare_width/num_divisions;
            memcpy(ss + row*subsquare_width*3, buffer_frame + (row_within_buffer_frame+row)*width*3 + column*3, subsquare_width*3);
        }
        subsquares[i] = ss;
    }

    return subsquares;
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
    int render_col = width-1;
    int render_row = 0;
    int width_pixels = width*3;
    unsigned char *tmp_pixel = (unsigned char*)malloc(3*sizeof(unsigned char));

    unsigned char *white_pixel = (unsigned char *)malloc(3*sizeof(unsigned char));
    memset(white_pixel, 255, 3);

    unsigned char *white_row = (unsigned char*)malloc(width*3*sizeof(unsigned char));
    memset(white_row, 255, width*3);

    //unsigned int num_divisions = 2;
    //unsigned char **subsquares = divideIntoSubsquares(buffer_frame, width, num_divisions);
    //int subsquare_width = width/num_divisions;

    switch(rotate_iteration) {
        case 1:
            // for (int subsquare_index = 0; subsquare_index < num_divisions*num_divisions; subsquare_index++) {
            //     unsigned char *rotated_subsquare = subsquares[subsquare_index];
            //     // /if (isSubsquareWhite(rotated_subsquare, subsquare_width)) break;
            //     //rotated_subsquare = rotateSubsquareCW(rotated_subsquare, subsquare_width);

            //     int subsquare_row_position = subsquare_index/num_divisions; // 0
            //     int subsquare_col_position = subsquare_index%num_divisions; // 1
            //     int start_row = subsquare_row_position*subsquare_width; //0
            //     int start_column = subsquare_col_position*subsquare_width; // 5
            //     // for (int row = 0; row < subsquare_width; row++) {
            //     //     memcpy(buffer_frame + (start_row+row)*width_pixels + start_column*3, rotated_subsquare + row*subsquare_width*3, subsquare_width*3);
            //     // }

            //     //break;
            //     if (subsquare_row_position < num_divisions/2 && subsquare_col_position < num_divisions/2) {
            //         // subsquare in upper left half
            //         // need to move subsquare to position in upper right half of buffer_frame
            //         start_column = (num_divisions - subsquare_col_position - 1)*subsquare_width; // 1 * 5  5;
            //         start_row = subsquare_row_position*subsquare_width; // 0
            //     } else if (subsquare_row_position < num_divisions/2 && subsquare_col_position >= num_divisions/2) {
            //         // subsquare in upper right half
            //         // need to move subsquare to bottom right half
            //         start_column = subsquare_col_position*subsquare_width; // 1 *5 = 5;
            //         start_row = (num_divisions - subsquare_row_position - 1)*subsquare_width; // 1*5=5;
            //    } else if (subsquare_row_position >= num_divisions/2 && subsquare_col_position < num_divisions/2) {
            //         // subsquare in bottom left half
            //         // need to move subsquare to top left half
            //         start_column = subsquare_col_position*subsquare_width;// 5
            //         start_row = (num_divisions - subsquare_row_position - 1)*subsquare_width; // 0
            //    } else if (subsquare_row_position >= num_divisions/2 && subsquare_col_position >= num_divisions/2) {
            //         // subsquare in bottom right half
            //         // need to move subsquare to bottom left half
            //         start_column = (num_divisions - subsquare_col_position - 1)*subsquare_width;//0
            //         start_row = subsquare_row_position*subsquare_width; //5
            //    }

            //     for (int row = 0; row < subsquare_width; row++) {
            //         memcpy(buffer_frame + (start_row+row)*width_pixels + start_column*3, rotated_subsquare + row*subsquare_width*3, subsquare_width*3);
            //     }
            // }

            // buffer_frame = rotateSubsquareCW(buffer_frame, width);
            // break;

            for (int row = 0; row < width>>1; row++) {
                for (int column = 0; column < height>>1; column++) {
                    int render_pos_a = render_row*width_pixels + render_col*3;
                    int render_pos_b = (height-row-1)*width_pixels + (width-column-1)*3;
                    int render_pos_c = (height-render_row-1)*width_pixels + (width-render_col-1)*3;
                    int original_pos = row*width_pixels + column*3;

                    if (memcmp(buffer_frame+original_pos, white_pixel, 3) != 0 ||
                        memcmp(buffer_frame+render_pos_c, white_pixel, 3) != 0 ||
                        memcmp(buffer_frame+render_pos_b, white_pixel, 3) != 0 ||
                        memcmp(buffer_frame+render_pos_a, white_pixel, 3) != 0) {

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
                    }
                    
                    render_row++;
                
                }
                render_row = 0;
                render_col--;
            }
            break;
        case 2:
            buffer_frame = rotateSubsquare180(buffer_frame, width);
            break;

            for (int row = 0; row < width>>1; row++) {
                if (memcmp(buffer_frame + row*width_pixels, white_row, width_pixels) != 0 ||
                    memcmp(buffer_frame + (height-row-1)*width_pixels, white_row, width_pixels) != 0 ) {
                    for (int column = 0; column < height; column++) {
                        int render_pos = (height-row-1)*width_pixels + (width-column-1)*3;
                        int original_pos = row*width_pixels + column*3;
                        memcpy(tmp_pixel, buffer_frame + original_pos, 3);
                        memcpy(buffer_frame + original_pos, buffer_frame + render_pos, 3);
                        memcpy(buffer_frame + render_pos, tmp_pixel, 3);
                    }
                } 
            }
            break;
        case 3:
            // buffer_frame = rotateSubsquareCCW(buffer_frame, width);
            // break;

            for (int row = 0; row < width>>1; row++) {
                for (int column = 0; column < height>>1; column++) {
                    int render_pos_a = render_row*width_pixels + render_col*3;
                    int render_pos_b = (height-row-1)*width_pixels + (width-column-1)*3;
                    int render_pos_c = (height-render_row-1)*width_pixels + (width-render_col-1)*3;
                    int original_pos = row*width_pixels + column*3;
                    if (memcmp(buffer_frame+original_pos, white_pixel, 3) != 0 ||
                        memcmp(buffer_frame+render_pos_c, white_pixel, 3) != 0 ||
                        memcmp(buffer_frame+render_pos_b, white_pixel, 3) != 0 ||
                        memcmp(buffer_frame+render_pos_a, white_pixel, 3) != 0) {

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
                    }

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
    //free(white_row);
    //free(subsquares);

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
        int rotate_iteration_mod = rotate_iteration%4;
        if (rotate_iteration_mod == 1) {
            rotate_iteration = 3;
        } else if (rotate_iteration_mod == 2) {
            rotate_iteration = 2;
        } else if (rotate_iteration_mod == 3) {
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

    unsigned char *white_row = (unsigned char*)malloc(width*3*sizeof(unsigned char));
    memset(white_row, 255, width*3);

    for (int row = 0; row < height>>1; row++) {
        if (memcmp(buffer_frame + row*width_pixels, white_row, width_pixels) != 0 ||
            memcmp(buffer_frame + (height-row-1)*width_pixels, white_row, width_pixels) != 0 ) {
            // copy row from top half of frame to temp
            memcpy(temp_row, buffer_frame+row*width_pixels, width_pixels);
            // copy row from bottom half to top half of frame
            memcpy(buffer_frame+row*width_pixels, buffer_frame+(height-row-1)*width_pixels, width_pixels);
            // copy row that was saved from top half to bottom half
            memcpy(buffer_frame+(height-row-1)*width_pixels, temp_row, width_pixels);
        }
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
    unsigned char *temp_row = (unsigned char*)malloc((width>>1)*3*sizeof(char));
    int width_pixels = width*3;

    unsigned char *white_row = (unsigned char*)malloc(width*3*sizeof(unsigned char));
    memset(white_row, 255, width*3);

    for (int row = 0; row < height; row++) {
        if (memcmp(buffer_frame + row*width_pixels, white_row, width_pixels) != 0) {
            for (int col = 0; col < width>>1; col++) {
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
            //memcpy(buffer_frame + row*width_pixels, temp_row, width_pixels/2);
        }
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

typedef enum {
    A = 65,
    D = 68,
    W = 87,
    S = 83,
    MX = 165,
    MY = 166,
    CW = 154,
    CCW = 134,
    VERIFY = 255,
} move;

// optimized key-value pairs
typedef struct {
    move m;
    int value;
} okv;

okv *optimize_sensor_values(struct kv *sensor_values, int sensor_values_count, int *new_count) {
    okv *optimized_sensor_values = (okv*)malloc(sensor_values_count*sizeof(okv));
    if (optimized_sensor_values == NULL) {
        return NULL;
    }

    bool is_mirrored_y = false;
    bool is_mirrored_x = false;
    int x_offset = 0;
    int y_offset = 0;
    int clockwise_rotation = 0;
    int pos = 0;

    for (int i = 0; i < sensor_values_count; i++) {
        int current_move = sensor_values[i].key[0] + sensor_values[i].key[1];
        // printf("move %s\n", sensor_values[i].key);
        int current_value = sensor_values[i].value;
        // printf("move %d value %d\n", current_move, current_value);

        switch(current_move) {
            case A: // left 
                // if (is_mirrored_x) x_offset += current_value;
                // else x_offset -= current_value;
                // break;
                if (is_mirrored_y) current_value *= -1;
                if (clockwise_rotation == 0) x_offset -= current_value;
                else if (clockwise_rotation == 1) y_offset -= current_value;
                else if (clockwise_rotation == 2) x_offset += current_value;
                else y_offset += current_value;
                break;
            case D: // right
                if (is_mirrored_y) current_value *= -1;
                if (clockwise_rotation == 0) x_offset += current_value;
                else if (clockwise_rotation == 1) y_offset += current_value;
                else if (clockwise_rotation == 2) x_offset -= current_value;
                else y_offset -= current_value;
                break;
            case S: // down
                if (is_mirrored_x) current_value *= -1;
                if (clockwise_rotation == 0) y_offset -= current_value;
                else if (clockwise_rotation == 1) x_offset += current_value;
                else if (clockwise_rotation == 2) y_offset += current_value;
                else y_offset -= current_value;
                break;
            case W: // up
                if (is_mirrored_x) current_value *= -1;
                if (clockwise_rotation == 0) y_offset += current_value;
                else if (clockwise_rotation == 1) x_offset -= current_value;
                else if (clockwise_rotation == 2) y_offset -= current_value;
                else y_offset += current_value;
                break;
            case CW:
                if (is_mirrored_x) current_value *= -1;
                if (is_mirrored_y) current_value *= -1;
                clockwise_rotation += current_value;
                clockwise_rotation = clockwise_rotation % 4;
                break;
            case CCW:
                clockwise_rotation -= current_value;
                break;
            case MX:
                is_mirrored_x = !is_mirrored_x;
                break;
            case MY:
                is_mirrored_y = !is_mirrored_y;
                break;
            default:
                break;
        }

        // printf("x_offset %d\n", x_offset);
        // printf("y_offset %d\n", y_offset);

        if ((i+1)%25 == 0) {
            if (x_offset < 0) {
                // printf("A %d\n", -1*x_offset);
                optimized_sensor_values[pos].m = A;
                optimized_sensor_values[pos].value = -1*x_offset;
                pos++; 
            } else if (x_offset != 0) {
                // printf("D %d\n", x_offset);
                optimized_sensor_values[pos].m = D;
                optimized_sensor_values[pos].value = x_offset;
                pos++;               
            }

            if (y_offset < 0) {
                // printf("S %d\n", -1*y_offset);
                optimized_sensor_values[pos].m = S;
                optimized_sensor_values[pos].value = -1*y_offset;
                pos++;  
            } else if (y_offset != 0) {                
                // printf("W %d\n", y_offset);
                optimized_sensor_values[pos].m = W;
                optimized_sensor_values[pos].value = y_offset;
                pos++;               
            }

            // collapse all mirroring
            if (is_mirrored_x) {
                optimized_sensor_values[pos].m = MX;
                optimized_sensor_values[pos].value = 1;
                pos++;
            }

            if (is_mirrored_y) {
                optimized_sensor_values[pos].m = MY;
                optimized_sensor_values[pos].value = 1;
                pos++;
            }

            // collapse all rotations
            if (clockwise_rotation < 0) {
                optimized_sensor_values[pos].m = CW;
                optimized_sensor_values[pos].value = -1*clockwise_rotation;
                pos++;
            } else if (clockwise_rotation != 0) {
                optimized_sensor_values[pos].m = CW;
                optimized_sensor_values[pos].value = clockwise_rotation;
                pos++;
            }

            // reset values
            is_mirrored_x = false;
            is_mirrored_y = false;
            x_offset = 0;
            y_offset = 0;
            clockwise_rotation = 0;

            // printf("VERIFY\n");

            // add a verify step since 25 of the original frames have passed
            optimized_sensor_values[pos].m = VERIFY;
            optimized_sensor_values[pos].value = 0;
            pos++;
        } 
    }

    *new_count = pos;

    return optimized_sensor_values;
}

// // remove whitespace around edges, keeps the image as a square
// // note width=height so new_width=new_height
unsigned char *remove_whitespace(unsigned char *frame_buffer, unsigned int width, unsigned int height, 
    unsigned int *new_width, unsigned int *left_offset, unsigned int *top_offset) {
    int begin_image_left = 0;
    int begin_image_right = 0;
    int begin_image_top = 0;
    int begin_image_bottom = 0;
    unsigned char *white_row = (unsigned char*)malloc(width*3*sizeof(unsigned char));
    memset(white_row, 255, width*3);

    // check for whitespace at top
    for (int row = 0; row < height; row++) {
        if (memcmp(white_row, frame_buffer + row*width*3, width*3) != 0) {
            begin_image_top = row;
            break;
        }
    }

    // check for whitespace at bottom
    for (int row = height - 1; row >= 0; row--) {
         if (memcmp(white_row, frame_buffer + row*width*3, width*3) != 0) {
            begin_image_bottom = row+1;
            break;
        }       
    }

    // rotate CW 90 degrees, check whitespace at top and bottom
    frame_buffer = processRotateCW(frame_buffer, width, height, 1);

    // check for whitespace at top (actually left)
    for (int row = 0; row < height; row++) {
        if (memcmp(white_row, frame_buffer + row*width*3, width*3) != 0) {
            begin_image_left = row;
            break;
        }
    }

    // check for whitespace at bottom (actually right)
    for (int row = height - 1; row >= 0; row--) {
         if (memcmp(white_row, frame_buffer + row*width*3, width*3) != 0) {
            begin_image_right = row+1;
            break;
        }       
    }

    // rotate back CCW
    frame_buffer = processRotateCCW(frame_buffer, width, height, 1);

    // figure out how much we can remove to crop the image into a square around the object
    int left_right_size = begin_image_right - begin_image_left;
    int top_bottom_size = begin_image_bottom - begin_image_top;
    unsigned int updated_width;

    if (top_bottom_size > left_right_size) {
        updated_width = top_bottom_size;
    } else {
        updated_width = left_right_size;
    }

    printf("left begin %d right begin %d\n", begin_image_left, begin_image_right);
    printf("top begin %d bottom begin %d\n", begin_image_top, begin_image_bottom);
    printf("left to right %d top to bottom %d\n", left_right_size, top_bottom_size);
    // remove excess whitespace
    //unsigned int updated_width = width - (current_removeable_pixels*2); // = new_height
    unsigned char *new_frame_buffer = (unsigned char*)malloc(updated_width*updated_width*3*sizeof(char));
    if (new_frame_buffer == NULL) {
        printf("error allocating for new frame buffer\n");
        return NULL;
    }

    int new_row = 0;
    while(height-begin_image_top < updated_width) {
        begin_image_top--;
    }

    // while(width-begin_image_right < updated_width) {
    //     begin_image_right;
    // }

    for (int row = begin_image_top; row < begin_image_top+updated_width; row++) {
        //printf("moving row %d\n", row);
        memcpy(new_frame_buffer + new_row*updated_width*3, frame_buffer + row*width*3 + begin_image_left*3, updated_width*3);
        new_row++;
    }

    *new_width = updated_width;
    *left_offset = begin_image_left;
    *top_offset = begin_image_top;

    return new_frame_buffer;
}

// readd whitespace to sides with x_offset and y_offset
unsigned char *readd_whitespace(unsigned char *optimized_frame_buffer, unsigned int original_width, unsigned int new_width, int x_offset, int y_offset) {
    unsigned char *frame_buffer = (unsigned char*)malloc(original_width*original_width*3*sizeof(unsigned char));
    memset(frame_buffer, 255, original_width*original_width*3);

    // calculate where to place cropped image on original image
    int start_row = y_offset;
    int start_column = x_offset;
    int end_row = new_width + y_offset;

    // place cropped image on white frame
    int inner_image_row = 0;
    for (int row = start_row; row < end_row; row++) {
        memcpy(frame_buffer + row*original_width*3 + start_column*3, optimized_frame_buffer + inner_image_row*new_width*3 , new_width*3);
        inner_image_row++;
    }

    return frame_buffer;
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
    //int processed_frames = 0;

    int new_count, left_offset, top_offset;
    okv *new_kv = optimize_sensor_values(sensor_values, sensor_values_count, &new_count);

    //printBMP(width, width, frame_buffer);

    int new_width;
    frame_buffer = remove_whitespace(frame_buffer, width, height, &new_width, &left_offset, &top_offset);
    //printBMP(new_width, new_width, frame_buffer);
    printf("new width %d left_offset %d top_offset %d\n", new_width, left_offset, top_offset);
    int x_offset = left_offset;
    int y_offset = top_offset;

    unsigned char *frame_buffer_with_whitespace;

    for (int i = 0; i < new_count; i++) {
        switch (new_kv[i].m) {
            case A: // left 
                //frame_buffer = processMoveLeft(frame_buffer, width, height, new_kv[i].value);
                x_offset -= new_kv[i].value;
                break;
            case D: // right
                //frame_buffer = processMoveRight(frame_buffer, width, height, new_kv[i].value);
                x_offset += new_kv[i].value;
                break;
            case W: // up
                //frame_buffer = processMoveUp(frame_buffer, width, height, new_kv[i].value);
                y_offset -= new_kv[i].value;
                break;
            case S: // down
                //frame_buffer = processMoveDown(frame_buffer, width, height, new_kv[i].value);
                y_offset += new_kv[i].value;
                break;
            case CW:
                //printBMP(width, height, frame_buffer);
                frame_buffer = processRotateCW(frame_buffer, width, height, new_kv[i].value);
                //printBMP(width, height, frame_buffer);
                if (new_kv[i].value == 1) {
                    int tmp = x_offset;
                    x_offset = -1*y_offset;
                    y_offset = tmp;
                } else if (new_kv[i].value == 2) {
                    y_offset = -1*y_offset;
                    x_offset = -1*x_offset;
                } else {
                    int tmp = x_offset;
                    x_offset = y_offset;
                    y_offset = -1*tmp;   
                }
                break;
            case CCW:
                frame_buffer = processRotateCCW(frame_buffer, width, height, new_kv[i].value);
                if (new_kv[i].value == 1) {
                    int tmp = x_offset;
                    x_offset = y_offset;
                    y_offset = -1*tmp; 
                } else if (new_kv[i].value == 2) {
                    y_offset = -1*y_offset;
                    x_offset = -1*x_offset;
                } else {
                    int tmp = x_offset;
                    x_offset = -1*y_offset;
                    y_offset = tmp;
                }
                break;
            case MX:
                frame_buffer = processMirrorX(frame_buffer, width, height, new_kv[i].value);
                y_offset = -1*y_offset;
                break;
            case MY:
                frame_buffer = processMirrorY(frame_buffer, width, height, new_kv[i].value);
                x_offset = -1*x_offset;
                break;
            case VERIFY:
                printf("new_width %d x_offset %d y_offset %d\n", new_width, x_offset, y_offset);
                frame_buffer_with_whitespace = readd_whitespace(frame_buffer, width, new_width, x_offset, y_offset);
                //printBMP(width, height, frame_buffer_with_whitespace);
                verifyFrame(frame_buffer_with_whitespace, width, height, grading_mode);
                x_offset = 0;
                y_offset = 0;
                //frame_buffer = remove_whitespace(frame_buffer, width, height, &new_width);
                break;    
            default: 
                break;    
        }
    }

    //frame_buffer = readd_whitespace(new_frame_buffer, width, new_width);
    free(new_kv);
    free(frame_buffer_with_whitespace);

//     for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
// //        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
// //               sensor_values[sensorValueIdx].value);
//         if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
//             frame_buffer = processMoveUp(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
//             frame_buffer = processMoveLeft(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
//             frame_buffer = processMoveDown(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
//             frame_buffer = processMoveRight(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
//             frame_buffer = processRotateCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
//             frame_buffer = processRotateCCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
//             frame_buffer = processMirrorX(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
//             frame_buffer = processMirrorY(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
// //            printBMP(width, height, frame_buffer);
//         }
//         processed_frames += 1;
//         if (processed_frames % 25 == 0) {
//             verifyFrame(frame_buffer, width, height, grading_mode);
//         }
//     }

    return;
}
