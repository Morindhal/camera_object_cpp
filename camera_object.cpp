#include <iostream>

struct camera_object
{
    camera_object(int tWidth, int tHeight, int tFramerate, int tQuality)
    : width(tWidth), height(tHeight), framerate(tFramerate), quality(tQuality)
    {}
    
    int width;
    int height;
    int framerate;
    int quality;
};

/*
 * Sets a resolution width given a already chosen height.
 */
void setWidth(camera_object *tCamera_object, const int sResolution[][2], int tWidth, int tResolutionSize)
{
    for (int n = 0 ; n<tResolutionSize ; n++)
    {
        if( sResolution[n][0] == tWidth )
        {
            if( tCamera_object->height < sResolution[n][1] )
            {
                if( n == 0 || sResolution[n-1][0] != tWidth )
                {
                    tCamera_object->width = tWidth;
                    tCamera_object->height = sResolution[n][1];
                    break;
                }
                else
                {
                    tCamera_object->width = tWidth;
                    tCamera_object->height = sResolution[n-1][1];
                    break;
                }
            }
            else if( n == tResolutionSize-1 && tCamera_object->height >= sResolution[n][1]  )
            {
                tCamera_object->width = tWidth;
                tCamera_object->height = sResolution[n][1];
                break;
            }
        }
    }
}


/*
 * allocates the highest value possible from tArray to tIntToCap without going over, defaults to the lowest value in tArray should the target value be too low.
 */
void lower_bound_integer_array(int *tIntToCap, const int tArray[], int size)
{
    int tBestFit = 0;
    if(size>=0)
        tBestFit = tArray[0];
    for(int i = 0 ; i < size ; i++)
        if(tArray[i] <= *tIntToCap )
            tBestFit = tArray[i];

    *tIntToCap = tBestFit;
}

/*
 * always sort camera capabilities by size, in case of resolution sort by width first and height second.
 * 
 * This function always sets the capabilities of a camera_object to as high as it can without exceeding the pre-programmed parameters.
 * When it comes to resolution it checks the width primarilly and height second.
 * The resolution-check:
 *      gives the highest pre-programmed width of a given target width.
 *      gives the highest pre-programmed height of the already chosen width.
 *          Should a target width be too low then the lowest available will be set.
 *          Should a target height be too high then the highest available will be set.
 * 
 * 
 */
void correct_camera_object(camera_object *tCamera_object)
{
    static const int sResolution[][2] = 
    {
        {160, 90},
        {160, 100},
        {160, 120},
        {176, 144},
        {240, 180},
        {320, 180},
        {320, 200},
        {320, 240},
        {480, 270},
        {480, 300},
        {480, 360},
        {640, 360},
        {640, 400},
        {640, 480},
        {800, 450},
        {800, 500},
        {800, 600}
    };
    static int sFramerate[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    static int sQuality[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};
    

    /*
     * The loop through sResolution to best match the target resolution
     */
    int resolution_count = ((sizeof(sResolution))/ (sizeof(int)) ) / 2;
    bool resolution_found = false;
    for (int i = 0 ; i<resolution_count && !resolution_found ; i++)
    {
        if ( tCamera_object->width < sResolution[i][0] )
        {
            if(i>0)
                setWidth(tCamera_object, sResolution, sResolution[i-1][0], resolution_count);
            else
                setWidth(tCamera_object, sResolution, sResolution[i][0], resolution_count);
            break;
        }
        else if ( i == resolution_count-1 && tCamera_object->width >= sResolution[i][0] )
        {
            setWidth(tCamera_object, sResolution, sResolution[i][0], resolution_count);
        }
    }
    
    /*
     * Set the framerate and quality to the highest possible value without going over the target value.
     */
    lower_bound_integer_array(&tCamera_object->framerate, sFramerate, sizeof(sFramerate) / sizeof(int));
    lower_bound_integer_array(&tCamera_object->quality, sQuality, sizeof(sQuality) / sizeof(int));
}

void printout(camera_object *tCamera_object)
{
    std::cout <<
        "\nResolution: " << tCamera_object->width << "x" << tCamera_object->height <<
        "\nFramerate: " << tCamera_object->framerate <<
        "\nQuality: " << tCamera_object->quality <<
        "\n";
}

int main()
{
    camera_object *my_camera_object = new camera_object(0, 95, 0, 2);
    printout(my_camera_object);
    correct_camera_object(my_camera_object);
    printout(my_camera_object);
    
    return 0;
}
