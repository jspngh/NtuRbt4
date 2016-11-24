#include "image.hpp"

Image::Image(string file_loc)
{
    cvImage = imread(file_loc, CV_LOAD_IMAGE_GRAYSCALE);

    reading_error = cvImage.data == NULL;
    if (reading_error)
        return;

    mImage = new int*[cvImage.rows];
    for (int i = 0; i < cvImage.rows; ++i)
    {
        mImage[i] = new int[cvImage.cols];
    }
    nr_regions = 0;
}

Image::Image(Mat image)
{
    cvImage = image;

    // if the image is a colored image, first convert
    if(image.channels() == 3)
    {
        Mat frame_bw;
        cvtColor(image, frame_bw, COLOR_BGR2GRAY);
        imshow("edges", frame_bw);
        cvImage = frame_bw;
    }
    
    reading_error = cvImage.data == NULL;
    if (reading_error)
        return;

    mImage = new int*[cvImage.rows];
    for (int i = 0; i < cvImage.rows; ++i)
    {
        mImage[i] = new int[cvImage.cols];
    }
    nr_regions = 0;
}

Image::~Image()
{
    for (int i = 0; i < cvImage.rows; ++i)
        delete[] mImage[i];
    delete[] mImage;

    for (int i = 0; i < nr_regions; ++i)
        delete regions[i];
    delete[] regions;
}

Mat Image::get_cvImage_result()
{
    return this->cvImage_result;
}

void Image::thresholding()
{
    for(int j=0; j<cvImage.rows; j++)
    {
        mImage[j] = new int[cvImage.cols];
        for (int i = 0; i < cvImage.cols; i++)
        {
            int value = (int)cvImage.at<uchar>(j,i);

            if (value > SEGMENTATION_THRESHOLD)
            {
                mImage[j][i] = 255;
            }
            else
            {
                mImage[j][i] = 0;
            }
        }
    }
}

void Image::segmentation()
{
    int i = 0;

    for (int k = 0; k < cvImage.rows; k++) // height
    {
        for (int j = 0; j < cvImage.cols; j++) // width
        {
            if (mImage[k][j] == 255)
            {
                i++;
                bool valid_region = grow_region(k, j, i);
                if (!valid_region)
                    i--;
            }
        }
    }
    nr_regions = i;
}

void Image::find_regions()
{
    if (nr_regions <= 0)
        return;

    regions = new Region*[nr_regions];
    for (int i = 1; i <= nr_regions; i++)
    {
        int top=cvImage.rows, bottom=0, left=cvImage.cols, right=0;
        for (int y = 0; y < cvImage.rows; y++)
        {
            for (int x = 0; x < cvImage.cols; x++)
            {
                if (mImage[y][x] == i)
                {
                    if (y < top)
                        top = y;

                    if (y > bottom)
                        bottom = y;

                    if (x < left)
                        left = x;

                    if (x > right)
                        right = x;
                }
            }
        }
        regions[i-1] = new Region(mImage, i, top, bottom, left, right);
    }
}

bool Image::grow_region(int k, int j, int i)
{
    int m = cvImage.rows; // height
    int n = cvImage.cols; // width
    stack<pair<int, int>> region;
    region.push(make_pair(-1, -1));
    pair<int, int> current_pos(k, j);

    int region_size = 0;

    while(!region.empty())
    {
        k = current_pos.first;
        j = current_pos.second;

        //top
        if (k > 0 && mImage[k-1][j] == 255)
        {
            region.push(make_pair(k-1, j));
            mImage[k-1][j] = i;
            region_size++;
        }
        //bottom
        if (k < (m-1) && mImage[k+1][j] == 255)
        {
            region.push(make_pair(k+1, j));
            mImage[k+1][j] = i;
            region_size++;
        }

        //right
        if (j < (n-1) && mImage[k][j+1] == 255)
        {
            region.push(make_pair(k, j+1));
            mImage[k][j+1] = i;
            region_size++;
        }

        //left
        if (j > 0 && mImage[k][j-1] == 255)
        {
            region.push(make_pair(k, j-1));
            mImage[k][j-1] = i;
            region_size++;
        }

        current_pos = region.top();
        region.pop();
        if(current_pos.first == -1 || current_pos.second == -1)
            break;
    }

    // if the region is too small, ignore it
    if (region_size < REGION_THRESHOLD)
    {
        for (int y = 0; y < cvImage.rows; ++y)
        {
            for (int x = 0; x < cvImage.cols; ++x)
            {
                if (mImage[y][x] == i)
                    mImage[y][x] = 0;
            }
        }
        return false;
    }
    return true;
}

void Image::print_debug_info()
{
    cout    << "Image [rows x cols]: ["
            << cvImage.rows << " x " << cvImage.cols << "]" << endl;

    for (int y = 0; y < cvImage.rows; ++y)
    {
        for (int x = 0; x < cvImage.cols; ++x)
        {
            cout << mImage[y][x] << ' ';
        }
        cout << endl;
    }

    cout << "Regions" << endl;
    for (int i = 0; i < nr_regions; ++i)
    {
        double x_c = regions[i]->centroid.first;
        double y_c = regions[i]->centroid.second;
        double phi = regions[i]->principle_angle;

        cout << "id: " << regions[i]->id
             << ", top: " << regions[i]->top
             << ", bottom: " << regions[i]->bottom
             << ", left: " << regions[i]->left
             << ", right: " << regions[i]->right
             << "\ncenter: " << x_c  << " " << y_c
             << "\nphi: " << phi
             << endl;
    }
}

void Image::print_region_metadata()
{

    cout << "Number of regions found: " << this->nr_regions << endl;
    for (int i = 0; i < nr_regions; ++i)
    {
        double x_c = regions[i]->centroid.first;
        double y_c = regions[i]->centroid.second;
        double phi = regions[i]->principle_angle;

        cout << round(x_c)  << " " << round(y_c) << " "
             << round((phi*180.0/3.141592)) << endl;
    }
}

void Image::display_region_metadata()
{
    // convert to colored image, so that the metadata can have colors
    cv::Mat cvImage_rgb(this->cvImage.size(), CV_8UC3);
    cv::cvtColor(this->cvImage, cvImage_rgb, CV_GRAY2RGB);

    for (int i = 0; i < nr_regions; i++)
    {
        double x_c = regions[i]->centroid.first;
        double y_c = regions[i]->centroid.second;

        double x_b = x_c - 100;
        double y_b = y_c - 100 * tan(regions[i]->principle_angle);
        double x_e = x_c + 100;
        double y_e = y_c + 100 * tan(regions[i]->principle_angle);

        Point center = Point(x_c,y_c);
        Point begin = Point(x_b, y_b);
        Point end = Point(x_e, y_e);

        cv::line(cvImage_rgb, begin, end, Scalar(255, 0, 0), 4);
        cv::drawMarker(cvImage_rgb, center, Scalar(0, 0, 255), cv::MARKER_SQUARE, 7, 2);
    }
    this->cvImage_result = cvImage_rgb;

    // namedWindow("processed image", CV_WINDOW_NORMAL);
    // resizeWindow("processed image", 1000, 1000);
    imshow( "processed image", cvImage_rgb);
    waitKey(0);
}
