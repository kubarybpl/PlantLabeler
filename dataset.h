
#ifndef DATASET_H
#define DATASET_H

#undef slots
#include<torch/script.h>
#include<torch/torch.h>
#define slots Q_SLOTS
#include<vector>
#include<string>
#include <io.h>
#include<opencv2/opencv.hpp>

class dataset:public torch::data::Dataset<dataset>
{

public:
    dataset(){
        for(int i = 0; i<10; i++){
            image_paths.push_back("C:/Users/Kuba/u-net/pytorch_dataset/test/X/image_60" + std::to_string(i) + ".png");
            masks_paths.push_back("C:/Users/Kuba/u-net/pytorch_dataset/test/Y/image_60" + std::to_string(i) + ".png");
        }
    }

    torch::data::Example<> get(size_t index) override{
        cv::Mat img = cv::imread(image_paths.at(index), cv::IMREAD_COLOR);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        torch::Tensor tensorImage = torch::from_blob(img.data, {img.rows, img.cols, img.channels()}, torch::kByte);
        tensorImage = tensorImage.permute({2, 0, 1});
        tensorImage = tensorImage.to(torch::kFloat32) / 255.0;
//        tensorImage = tensorImage.unsqueeze(0);

        cv::Mat mask = cv::imread(masks_paths.at(index), cv::IMREAD_GRAYSCALE);
//        cv::cvtColor(mask, mask, cv::COLOR_BGR2RGB);
        torch::Tensor tensorMask = torch::from_blob(mask.data, {mask.rows, mask.cols, mask.channels()}, torch::kByte);
        tensorMask = tensorMask.permute({2, 0, 1});
        tensorMask = tensorMask.to(torch::kFloat32);
        tensorMask = tensorMask.unsqueeze(0);

        return {tensorImage.clone(), tensorMask.clone()};
    }

    torch::optional<size_t> size() const override {
        return image_paths.size();
    }
private:
    std::vector<std::string> image_paths;
    std::vector<std::string> masks_paths;
};

#endif // DATASET_H
