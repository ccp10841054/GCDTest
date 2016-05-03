//
//  ReadMe.h
//  GCDTest
//
//  Created by sandy on 16/5/3.
//  Copyright © 2016年 sandy. All rights reserved.
//


http://blog.csdn.net/lengshengren/article/details/12905787

http://www.th7.cn/Program/IOS/201411/315273.shtml

http://blog.csdn.net/crycheng/article/details/22214617

http://supershll.blog.163.com/blog/static/37070436201282425950436/
http://www.cnblogs.com/tangbinblog/p/4133481.html
http://wenku.baidu.com/link?url=OxtC50CMEmvUORcl-_FOvQAFvr-9-25NGwD-MbgfF09mTH-ro6H5u9mdtk6P8iGMpXtoBRXBnygf5pmhugAlr8Cz5lbasgqO7IUo81-cRLu

#import <Foundation/Foundation.h>

@interface ReadMe : NSObject

@end

//GCD和NSOpretion比较;
//
//1 GCD不可以取消或中断任务,NSOpretion可以;
//
//GCD Queue 分为三种：
//
//1，The main queue  ：主队列，主线程就是在个队列中。
//
//2 Global queues  全局并发队列。
//
//3用户队列:是用函数 dispatch_queue_create 创建的自定义队列
//
//
//
//dispatch_sync 和  dispatch_async 区别
//
//dispatch_async(queue,block)  async 异步队列dispatch_async 函数会立即返回, block会在后台异步执行。
//
//dispatch_sync(queue,block)   sync 同步队列dispatch_sync 函数不会立即返回，及阻塞当前线程,等待 block同步执行完成。
//
//
//
//
//1)
//- (void)viewDidLoad
//{
//    [super viewDidLoad];
//    NSLog(@"=================4");
//    dispatch_sync(dispatch_get_main_queue(), ^{
//        NSLog(@"=================5");
//    });
//    NSLog(@"=================6");
//}
//
//分析上面代码
//输出结果：1
//viewDidLoad 在主线程中， 及在
//dispatch_get_main_queue() 中，执行到sync 时 向
//dispatch_get_main_queue()插入 同步 threed1.
//
//sync 会等到 后面block 执行完成才返回， sync 又再 dispatch_get_main_queue() 队列中，
//它是串行队列，sync 是后加入的，前一个是主线程，
//所以 sync 想执行 block 必须等待主线程执行完成，主线程等待 sync 返回，去执行后续内容。
//
//照成死锁，sync 等待mainThread 执行完成， mianThread 等待sync 函数返回。
//
//2)
//- (void)viewDidLoad
//{
//    [super viewDidLoad];
//    
//    dispatch_async(dispatch_get_global_queue(0, 0), ^{
//        
//        NSLog(@"=================1");
//        
//        dispatch_sync(dispatch_get_main_queue(), ^{
//            NSLog(@"=================2");
//            
//        });
//        NSLog(@"=================3");
//        
//        
//    });
//    
//}
//复制代码
//
//输出结果：1 2 3
//
//首先： async 在主线程中  创建了一个异步线程 加入  全局并发队列，async 不会等待block 执行完成，立即返回，
//
//1，async 立即返回， viewDidLoad 执行完毕，及主线程执行完毕。
//2，同时，全局并发队列立即执行异步 block ， 打印 1， 当执行到 sync 它会等待 block 执行完成才返回， 及等待
//dispatch_get_main_queue() 队列中的 mianThread 执行完成， 然后才开始调用block 。
//
//因为1 和 2 几乎同时执行，因为2 在全局并发队列上， 2 中执行到sync 时 1 可能已经执行完成或 等了一会，mainThread 很快退出， 2 等已执行后续内容。
//
//如果阻塞了主线程，2 中的sync 就无法执行啦，mainThread 永远不会退出， sync 就永远等待着，
//
//3）
//- (void)viewDidLoad
//{
//    [super viewDidLoad];
//    
//    dispatch_async(dispatch_get_global_queue(0, 0), ^{
//        NSLog(@"=================1");
//        dispatch_sync(dispatch_get_main_queue(), ^{
//            NSLog(@"=================2");
//        });
//        NSLog(@"=================3");
//    });
//    NSLog(@"==========阻塞主线程");
//    while (1) {
//    }
//    NSLog(@"========2==阻塞主线程");
//    
//}
//复制代码
//打印如下：
//
//2014-11-30 17:56:22.296 Test[6108:379350] =================1
//
//2014-11-30 17:56:22.296 Test[6108:379231] ==========阻塞主线程
//
//永远等着。。。。。
