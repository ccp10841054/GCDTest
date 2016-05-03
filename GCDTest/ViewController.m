//
//  ViewController.m
//  GCDTest
//
//  Created by sandy on 16/5/3.
//  Copyright © 2016年 sandy. All rights reserved.
//

#import "ViewController.h"

@interface ViewController (){
    NSOperationQueue * task;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    task = [[NSOperationQueue alloc]init];
    task.maxConcurrentOperationCount = 5;

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)button1Action:(id)sender {
//    NSLog(@"1");
//    dispatch_sync(dispatch_get_main_queue(), ^{
//        NSLog(@"2");
//    });
//    NSLog(@"3");
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        NSLog(@"=================1");
        dispatch_sync(dispatch_get_main_queue(), ^{
            NSLog(@"=================2");
        });
        NSLog(@"=================3");
    });
    NSLog(@"==========阻塞主线程");
    while (1) {
       //主线程一直为真，一直没有返回，去掉这个就ok
    }

    /*输出结果为“1”
     *被死锁在主线程，这是又Bug的，死锁原因:
     *1:dispatch_sync在等待block语句执行完成，而block语句需要在主线程里执行，所以dispatch_sync如果在主线程调用就会造成死锁，如同自己等待自己点外卖
     *2:dispatch_sync是同步的，本身就会阻塞当前线程，也即主线程。而又往主线程里塞进去一个block，所以就会发生死锁
     *3.dispatch_sync(dispatch_get_main_queue(), ^{ });一般都是用在异步里面，即用在

     */
   

}
- (IBAction)button2Action2:(id)sender {
    
      NSLog(@"1");
    dispatch_async(dispatch_get_main_queue(), ^{
    //async 在主线程中 创建了一个异步线程 加入 全局并发队列，async不会等待block 执行完成，立即返回
        NSLog(@"2");//不会造成死锁
    });
    NSLog(@"3");
    //输出结果为 1 3 2
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0), ^{
        dispatch_sync(dispatch_get_main_queue(), ^{
            NSLog(@"4");
        });
    });
    
}
- (IBAction)button3Action:(id)sender {
    
    NSLog(@"1");
    NSBlockOperation * op1 = [[NSBlockOperation alloc]init];
    [op1 addExecutionBlock:^{
        NSLog(@"2");
    }];
    [task addOperation:op1];
    
    NSBlockOperation * op2 = [[NSBlockOperation alloc]init];
    [op2 addExecutionBlock:^{
        dispatch_sync(dispatch_get_main_queue(), ^{
            NSLog(@"3");
        });
    }];
    [task addOperation:op2];

    NSLog(@"4");
    
   /* dispatch_sync(queue1, block);
    假设该语句在queue0中执行，则会阻塞(等待)queue0直到queue1中的block执行结束。
    当queue0和queue1不同时，queue0与queue1为并行执行，此时queue0等待，queue1执行block，queue0与queue1互不影响，直到queue1的block执行结束，queue0阻塞也结束，即dispatch_sync语句返回。
    当queue0和queue1相同（且为串行队列Serial Dispatch Queue）时，即dispatch_sync与block在同一个队列中，此时queue0已经阻塞，不可能再执行block，更无法等待block执行结束，因此造成死锁。
    */
    
}

@end
