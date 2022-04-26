#include "captcha.h"
#include <memory>
#include <iostream>

#include "captcha_managner.hpp"


int main(){

    //auto t = std::make_unique<captcha<364,355>>(njnaruto_bmf);
    //std::cout << (t->bmf_).width_<< std::endl;
    //std::cout << (t->bmf_).height_ << std::endl;
    //std::cout << (t->bmf_).buff_->size()<< std::endl;
    //std::cout << (t->image_).buff_.size()<< std::endl;
    //std::cout << (t->image_).width_<< std::endl;
    //std::cout << (t->image_).height_<< std::endl;
    //for(int i=0;i<(t->bmf_).buff_->size();++i){
        //(t->image_).buff_[i] = (t->bmf_).buff_->at(i);
    //}
    //int x,y;
    //x = 0,y=0;
    //std::cout << x <<" "<< y << std::endl;
    //std::cout << (t->image_).pos(x,y) << std::endl;

    //x = 0,y=1;
    //std::cout << x <<" "<< y << std::endl;
    //std::cout << (t->image_).pos(x,y) << std::endl;

    //x = 1,y=1;
    //std::cout << x <<" "<< y << std::endl;
    //std::cout << (t->image_).pos(x,y) << std::endl;
    //for(int i=0;i<=100;++i){
        //for(int j=0;j<=100;++j){
            //RGBA a;
            //a.value = 0xccccccff;
            //(t->image_).set_pixel(a, i, j);
        //}
    //}
    //t->set_char('a', 0, 0);
    //t->save_to_file();

    //for(int i=1;i<=100;++i){
        //auto x = make_njnaruto_captcha();
        //auto str = x->get_rand_str();
        //std::cout << str << std::endl;
        //x->make_captcha();
        ////x->set_char('a',0,0);
        //x->save_to_file();
    ////}
    auto ret = 
    captcha_manager::get().create_captcha_to_pngbase64("hello");
    std::cout << ret << std::endl;
    auto cap = captcha_manager::get().get_captcha("hello");
    std::cout << cap << std::endl;
    return 0;
}
