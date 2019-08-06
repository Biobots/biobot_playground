package main

import (
	"fmt"
	"time"
)

func main() {
	ch := make(chan int)
	timeout := make(chan bool, 1)
	go func() {
		fmt.Println("timing")
		time.Sleep(10)
		timeout <- true
	}()
	go func() {
		fmt.Println("running")
		ch <- 0
		fmt.Println("sent")
	}()
	fmt.Println("waiting")
	select {
	case <-ch:
		fmt.Println("done")
	case <-timeout:
		fmt.Println("timeout")
	}
}
