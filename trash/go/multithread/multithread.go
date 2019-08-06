package main

import (
	"fmt"
	"runtime"
	"sync"
	"time"
)

var (
	counter int
	wg      sync.WaitGroup
)

func running() {
	var times int
	for {
		times++
		fmt.Println("tick", times)
		time.Sleep(time.Second)
	}
}

func intCount(id int, lock *sync.Mutex) {
	defer wg.Done()
	for count := 0; count < 2; count++ {
		lock.Lock()
		value := counter
		runtime.Gosched()
		value++
		counter = value
		lock.Unlock()
	}
}

func main() {
	//go running()
	//go func() {
	//	for {
	//		fmt.Println("aa")
	//		time.Sleep(time.Second * 2)
	//	}
	//}()
	//var input string
	//fmt.Scanln(&input)
	lock := &sync.Mutex{}
	wg.Add(2)
	go intCount(1, lock)
	go intCount(2, lock)
	wg.Wait()
	fmt.Println("final count:", counter)
}
