#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stack>
#include<opencv2/imgcodecs/imgcodecs.hpp>
class Uni
{
public:
	cv::Mat mask;//标签矩阵.
	cv::Point_<int> p; // x为行，y为列
	std::stack<cv::Point_<int>> point;// 点栈
	void SeedFill(cv::Mat& image, cv::Mat& label);
};

//思路如下
//1.image为原图像，label为连通域的序号图像
// 
//2.创建一个伴生图像mask，它的作用就在像素在入栈时的判断功能
// 
//3.如果mask在对应位置的值为1，则说明改像素已经在栈中或者被标记过了.
// 
// 4.如果mask的标签为0，说明未被标记或者没被压入栈中，则需要压入栈中
// 
// 
// 
// 
// 注意事项：
// 像素检查邻接的时候需要格外注意
// 如果边界也有连通域的像素，那么它的四联通在不被约束的情况下也会检查
// 注意即使不需要边界上的连通域像素，它也会被压入栈中
// 因为它可以通过与边界连通域像素邻接的像素来压入栈中
// 
// 所以我们在检查前必须查看它是否在边界上
// 
// 分类讨论：
// a.如果它在左边界，那么它的左边像素不能检查 
// b.如果它在右边界，那么它的右边像素不能检查
// c.如果他在上边界，那么它的上一行像素不能检查
// d.如果它在下边界，那么它的下一行像素不能检查
// 
// 
// 还要注意的是,像素检查必须是严格按照左右上下来检查  否则容易出现错误.
// 
//                                    up(Third)
// 
//        right(Second)        pixel           left(First)
// 
//                                    down(Fourth)
// 
//endif





void Uni::SeedFill(cv::Mat& image, cv::Mat& lab)
{

        if(!lab.empty()) lab.release();
	lab = cv::Mat(image.size(),CV_32SC1);
	lab.setTo(0);

	//用来指向二值化图像的上中下三行的指针
	// 因为image图像不做改变，所以只读
	//endif
	const uchar* pixel = NULL;
	const uchar* pixelup = NULL;
	const uchar* pixeldown = NULL;


	//栈中的像素的对应像素
	int* labptr = NULL;



	//用于装像素的坐标栈
	int label = 0;

	//标记矩阵，查看对应像素是否压入栈中，压入栈中的像素其值为1；
	mask=cv::Mat(image.size(), CV_8UC1, cv::Scalar(0));


	//对应像素标签
	uchar* maskptr = NULL;

	//四联通需要上中下三行操作.
	uchar* maskptrup = NULL;   //上
	uchar* maskpptr = NULL;   //中
	uchar* maskptrdown = NULL;  //下




	for (int i = 0; i < image.rows; i++)
	{
	    pixel = image.ptr<uchar>(i);//中
	    
		//对应标签矩阵的像素
		maskptr = mask.ptr<uchar>(i);


		for (int j = 0; j < image.cols ; j++)
		{

			if (*(pixel+j) == 255  &&  *(maskptr+j) != 1)
			{
			    point.push(cv::Point_<int>(i, j));  // i=x j=y
			    label+=1;
				*(maskptr + j) = 1;
			     while (!point.empty())
			     {
					 
					 p = point.top();
					
			    		//该点为label
			    	labptr = lab.ptr<int>(p.x);

					maskptr = mask.ptr<uchar>(p.x);

					*(maskptr + p.y) = 1; //标记
			    	*(labptr + p.y) = label; //连通域序号
			    
			    		//出栈
					point.pop();

					   //左边界
			    	   if ( p.y >=1 ) 
			    	    {
			    	   pixel = image.ptr<uchar>(p.x);
			    	    	maskpptr = mask.ptr<uchar>(p.x);
			    	    	if (*(pixel + p.y - 1) == 255  &&  *(maskptr + p.y - 1) !=1)
			    	    	{
			    	    		point.push(cv::Point_<int>(p.x, p.y - 1));
			    	    		*(maskptr + p.y - 1) = 1;
			    	    	}
			    	    }
					   //右边界
					   if (p.y <= image.cols - 2) 
					   {
						   pixel = image.ptr<uchar>(p.x);
						   maskpptr = mask.ptr<uchar>(p.x);
						   if (*(pixel + p.y + 1) == 255 && *(maskptr + p.y + 1) != 1)
						   {
							   point.push(cv::Point_<int>(p.x, p.y + 1));
							   *(maskptr + p.y + 1) = 1;
						   }

					   }
					   //上边界
					   if (p.x >= 1)  
					   {
						   pixelup = image.ptr<uchar>(p.x - 1);
						   maskptrup = mask.ptr<uchar>(p.x - 1);
						   if (*(pixelup + p.y) == 255 && *(maskptrup + p.y) != 1)
						   {
							   point.push(cv::Point_<int>(p.x - 1, p.y));
							   *(maskptrup + p.y) = 1;
						   }
					   }
					   //下边界
					   if (p.x <= image.rows - 2) 
					   {
						   pixeldown = image.ptr<uchar>(p.x + 1);
						   maskptrdown = mask.ptr<uchar>(p.x + 1);
						   if (*(pixeldown + p.y) == 255 && *(maskptrdown + p.y) != 1)
						   {
							   point.push(cv::Point_<int>(p.x + 1, p.y));
							   *(maskptrdown + p.y) = 1;
						   }
					   }
			     }//while endif
			}
		}
	}
}

