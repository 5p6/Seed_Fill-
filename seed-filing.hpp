#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stack>
#include<opencv2/imgcodecs/imgcodecs.hpp>
class Uni
{
public:
	cv::Mat mask;//��ǩ����.
	cv::Point_<int> p; // xΪ�У�yΪ��
	std::stack<cv::Point_<int>> point;// ��ջ
	void SeedFill(cv::Mat& image, cv::Mat& label);
};

//˼·����
//1.imageΪԭͼ��labelΪ��ͨ������ͼ��
// 
//2.����һ������ͼ��mask���������þ�����������ջʱ���жϹ���
// 
//3.���mask�ڶ�Ӧλ�õ�ֵΪ1����˵���������Ѿ���ջ�л��߱���ǹ���.
// 
// 4.���mask�ı�ǩΪ0��˵��δ����ǻ���û��ѹ��ջ�У�����Ҫѹ��ջ��
// 
// 
// 
// 
// ע�����
// ���ؼ���ڽӵ�ʱ����Ҫ����ע��
// ����߽�Ҳ����ͨ������أ���ô��������ͨ�ڲ���Լ���������Ҳ����
// ע�⼴ʹ����Ҫ�߽��ϵ���ͨ�����أ���Ҳ�ᱻѹ��ջ��
// ��Ϊ������ͨ����߽���ͨ�������ڽӵ�������ѹ��ջ��
// 
// ���������ڼ��ǰ����鿴���Ƿ��ڱ߽���
// 
// �������ۣ�
// a.���������߽磬��ô����������ز��ܼ�� 
// b.��������ұ߽磬��ô�����ұ����ز��ܼ��
// c.��������ϱ߽磬��ô������һ�����ز��ܼ��
// d.��������±߽磬��ô������һ�����ز��ܼ��
// 
// 
// ��Ҫע�����,���ؼ��������ϸ����������������  �������׳��ִ���.
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


	lab = cv::Mat(image.size(),CV_32SC1);
	lab.setTo(0);

	//����ָ���ֵ��ͼ������������е�ָ��
	// ��Ϊimageͼ�����ı䣬����ֻ��
	//endif
	const uchar* pixel = NULL;
	const uchar* pixelup = NULL;
	const uchar* pixeldown = NULL;


	//ջ�е����صĶ�Ӧ����
	int* labptr = NULL;



	//����װ���ص�����ջ
	int label = 0;

	//��Ǿ��󣬲鿴��Ӧ�����Ƿ�ѹ��ջ�У�ѹ��ջ�е�������ֵΪ1��
	mask=cv::Mat(image.size(), CV_8UC1, cv::Scalar(0));


	//��Ӧ���ر�ǩ
	uchar* maskptr = NULL;

	//����ͨ��Ҫ���������в���.
	uchar* maskptrup = NULL;   //��
	uchar* maskpptr = NULL;   //��
	uchar* maskptrdown = NULL;  //��




	for (int i = 0; i < image.rows; i++)
	{
	    pixel = image.ptr<uchar>(i);//��
	    
		//��Ӧ��ǩ���������
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
					
			    		//�õ�Ϊlabel
			    	labptr = lab.ptr<int>(p.x);

					maskptr = mask.ptr<uchar>(p.x);

					*(maskptr + p.y) = 1; //���
			    	*(labptr + p.y) = label; //��ͨ�����
			    
			    		//��ջ
					point.pop();

					   //��߽�
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
					   //�ұ߽�
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
					   //�ϱ߽�
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
					   //�±߽�
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

