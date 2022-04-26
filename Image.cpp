//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"



bool Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary | std::ios::in); // need to spec. binary & input mode for Windows users
    try
    {
        if (ifs.fail())
        {
            throw ("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw ("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h];  // this will throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines if necessary until we get to the binary data

        unsigned char pixel_buffer[3]; // buffer to store one pixel, with three channels red,green and blue

        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < (w * h); ++i)
        {
            ifs.read(reinterpret_cast<char *>(pixel_buffer), 3);
            this->pixels[i].r = pixel_buffer[0]; // / 255.f;
            this->pixels[i].g = pixel_buffer[1]; // / 255.f;
            this->pixels[i].b = pixel_buffer[2]; // / 255.f;
        }
        ifs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
    return true;
}
bool Image::loadRaw(string filename)
{
    return false;
}
bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0)
    {
        fprintf(stderr, "Can't save an empty image\n");
        return true;
    }
    std::ofstream ofs;
    try
    {
        ofs.open(filename, std::ios::binary|std::ios::out); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->w << " " <<  this->h << "\n255\n";

        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->w *  this->h; ++i)
        {
//            r = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].r) * 255);
//            g = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].g) * 255);
//            b = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].b) * 255);
//            ofs << r << g << b;

            ofs.write(reinterpret_cast<char *>(&this->pixels[i].r),1);    // write 1 byte;
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].g),1);    // write 1 byte;
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].b),1);    // write 1 byte;
        }
        ofs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }

    return true;
}

void Image::filterRed()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        pixels[i].b = 0;
        pixels[i].g = 0;
    }
}
void Image::filterGreen()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        pixels[i].b = 0;
        pixels[i].r = 0;
    }
}
void Image::filterBlue()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        pixels[i].g = 0;
        pixels[i].r = 0;
    }
}
void Image::greyScale()
{
    for(int i = 0; i <= w*h-1 ;i++)
    {
        // Code given by andrew
        float grayScale = (pixels[i].r + pixels[i].g + pixels[i].b) /3;
        pixels[i].r = grayScale;
        pixels[i].g = grayScale;
        pixels[i].b = grayScale;
    }
}
void Image::flipHorizontal()
{
    //Makes a temp image that all our calculations are rin on
    Image *tmpImage = new Image();
    *tmpImage->pixels = *pixels;

    int y;//Variable to move along the height
    int x;//Variable to move along the width

    //Loops over all the pixels in our temp image
    for (int total = 0; total<=(h*w) ; total+=w)
    {
        //Loop along the width(row)
        for (int x = 0; x < w ; x++)
        {
            //Temp image pixels are = pixels of the whole image, -x to get the last pixel on the row
            tmpImage->pixels[y+x]  = pixels[(total+w) - (x+1)];
        }
        //y now takes on the value of total
        y = total;
    }

    //The pixels displayed are now take on the values of tmpImage pixels which shows
    //the image flipped on the horizontal
    pixels = tmpImage->pixels;
}
//Follow the same approach as horizontal, only we are flipping on the x-axis, rather than the height
void Image::flipVertically()
{
    Image *tmpImage = new Image();
    *tmpImage->pixels = *pixels;

    int y = (h*w);
    int x;

    for (int total = 0; total<=(h*w) ; total+=w)
    {
        if (y > 0)
        {
            y = y - w;
        }

        for (int x = 0; x < w ; x++)
        {
            tmpImage->pixels[total+x] = pixels[(y+x)];
        }
    }
    pixels = tmpImage->pixels;
}
void Image::AdditionalFunction2()
{
    Image zImage(w*2 , h*2 );//Make a temp image 2 times the size as the displayed one

    // ZOOM the image, could also be image RESIZE
    // the idea is to create a larger image, grab a portion of it and display in the
    // main image "zoom". we do this by getting each pixel in the main image and
    // making that 4 square on the larger one.

    int nextRow = 0, startPos = 0, step = 0, row = 0;// Zoom variables

    for (int i = 0; i < (w * h); ++i)
    {
        if (i%w==0)// check to see if we are at the end of a row
        {
            row = i*2;//next row on zoomed image
            nextRow = row+zImage.w;
        }
        zImage.pixels[i+step+row] = pixels[i];//This double all pixels on the x axis
        zImage.pixels[i+step+row+1] = pixels[i];

        zImage.pixels[i+step+nextRow] = pixels[i];//This double all pixels on the x axis on the next row
        zImage.pixels[i+step+nextRow+1] = pixels[i];//nextRow is the row below the current

        step+=1;//current pixel plus the one beside it
    }
    row=0;//reset row
    startPos = ((zImage.w*zImage.h)/4)-(w/2);//-(h*w);//start in the middle

    //zImage->savePPM("test.ppm");// testing

    for (int i = 0; i < (w * h); ++i)
    {
        if (i%w==0)// check to see if we are at the end of a row
        {
            row += w;//next row on zoomed image
        }
        pixels[i]=zImage.pixels[i+row+startPos];
    }
}
void Image::AdditionalFunction3()
{
    float depth = 20;
    float intensity = 2;
    for(int i = 0; i <= w*h-1 ;i++)
    {
        float average = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].r = average + (depth*2);
        pixels[i].g = average + depth;
        pixels[i].b = average - intensity;
        if(pixels[i].r > 255)
        {
            pixels[i].r = 255;
        }
        if(pixels[i].g > 255)
        {
            pixels[i].g = 255;
        }
        if(pixels[i].g > 255)
        {
            pixels[i].b = 255;
        }
        if(pixels[i].b > 255)
        {
            pixels[i].b = 255;
        }
        if(pixels[i].b < 0)
        {
            pixels[i].b = 0;
        }
    }

}

void Image::AdditionalFunction1()
{
    //Code heavily based on delboy8080 starter github code
    Image blurImage;
    blurImage.pixels = pixels;

    //Variables for new r g b values
    float rTotal = 0, gTotal=0, bTotal=0;
    //Loop to interate along the height
    for(int y = 0 ; y < h; y++)
    {
        //Loop to interate along the width
        for(int x = 0 ; x < w; x++)
        {
            //All variables are set to zero
            rTotal = 0; gTotal = 0; bTotal= 0;
            //If at top left, or at the end of a row or column continue
            if(x==0||y==0||x==w-1||y==h-1)
                continue;
            else
            {
                //Here we get each pixel, and swap it with the one next to it in the array
                Rgb neighbours[] =
                        {pixels[(y-1)*w+(x-1)],
                         pixels[(y-1)*w+x],
                         pixels[(y-1)*w+x+1],
                         pixels[(y)*w+x-1],
                         pixels[(y)*w+x],
                         pixels[(y)*w+x+1],
                         pixels[(y+1)*w+(x-1)],
                         pixels[(y+1)*w+x],
                         pixels[(y+1)*w+x+1]};

                for(Rgb p: neighbours)
                {
                    //Here the pixels get assigned there new value after being switched in the array
                    rTotal += p.r;
                    gTotal += p.g;
                    bTotal += p.b;
                }

                //Not really sure why you must divide by 9
                blurImage.pixels[y*w+x].r=rTotal/9;
                blurImage.pixels[y*w+x].g=gTotal/9;
                blurImage.pixels[y*w+x].b=bTotal/9;
            }

            //Display the image being blurred once all calculations are done to the temp image
            pixels = blurImage.pixels;
        }
    }
}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}