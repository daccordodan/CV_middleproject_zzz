#include <imageIterator.h>

ImageIterator::ImageIterator(const std::string& path){
    if(!path.empty())
        this->path="../dataset/data/"+path;
    currentIndex=0;
    if (cv::imread(path+"0000.png").empty())
        ext=".jpg";
    else
        ext=".png";
}

void ImageIterator::next(cv::Mat& image){
    std::string completePath = path+"/"+std::to_string(currentIndex)+ext;
    if(hasNext()){
        image=cv::imread(completePath);
        ++currentIndex;
    }
}

bool ImageIterator::hasNext(){
    if (cv::imread(path+std::to_string(currentIndex)+ext).empty())
        return false;
    return true;
}

std::string ImageIterator::getPath() const{
    return path;
}

int ImageIterator::getCurrentIndex() const{
    return currentIndex;
}