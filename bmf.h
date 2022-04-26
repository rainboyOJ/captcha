
// bitmap font
//
#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <iostream>

struct char_info {
    int id,x,y,width,height;
    //int xoffset,yoffset,xadvance,page,chnl;
};


using uc = unsigned char;
union RGBA {
    u_int8_t rgba[4];
    struct {
        u_int8_t r,g,b,a;
    } color;
    u_int32_t value;
};
class bmf {
public:
    
    using Buff    = const std::vector<u_int8_t> *;
    using infoPtr = const std::vector<char_info> *;
    
    explicit bmf (size_t width,size_t height,Buff bf,infoPtr i_)
        :width_{width},height_{height},
        buff_{bf},info_{i_}
    {
        for (auto& e : char_index)  e = -1;
        for (int i = 0;i< info_->size();++i) {
            char c  = info_->at(i).id;
            if( !in_charr_array(c, 'I','i','o','\n','\r','O','0','1','l','L',' ','G','g')) {
                if( info_->at(i).height != 0 &&  info_->at(i).width!= 0 )

                char_set.push_back(c);
            }
            char_index[c] = i;
        }
    }
    inline size_t pos(size_t x,size_t y) const{
        return (x +  y*width_) <<2;
    }

    template<typename... Args>
    bool in_charr_array(char c,Args... args){
        return ( (c == args)|| ... );
    }
    
    RGBA get_pixel(size_t x,size_t y) const{
        RGBA t;
        if( x >= width_ && y >= height_ )
            return {0};
        t.rgba[0] = (*buff_)[pos(x,y)];
        t.rgba[1] = (*buff_)[pos(x,y)+1];
        t.rgba[2] = (*buff_)[pos(x,y)+2];
        t.rgba[3] = (*buff_)[pos(x,y)+3];
        return t;
    }

    //std::pair<int, int> real_

    //把id 从x,y这个位置写入到image
    //得到char info
    std::tuple<bool,const char_info *>
        get_char(char c) const{
        if( char_index[c] == -1 )
            return {false,nullptr};
        return {true,&((*info_)[char_index[c]])};
    }
    std::string char_set;
    int char_index[258];
    size_t width_,height_;
    Buff buff_;
    infoPtr info_;
};
