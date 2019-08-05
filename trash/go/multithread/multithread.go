package main

import(
	"fmt"
	"time"
)

func running() {
	var times int
	for {
		times++
		fmt.Println("tick", times)
		time.Sleep(time.Second)
	}
}

func main() {
	go running()
	go func() {
		for {
			fmt.Println("aa")
			time.Sleep(time.Second * 2)
		}
	}()
	var input string
    fmt.Scanln(&input)
}