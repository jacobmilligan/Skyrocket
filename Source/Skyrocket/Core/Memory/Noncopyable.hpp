//
//  Noncopyable.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 3/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

namespace sky {


class Noncopyable {
protected:
	Noncopyable() {};
private:
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};


}