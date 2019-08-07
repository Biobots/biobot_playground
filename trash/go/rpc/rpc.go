package main

import (
	"errors"
	"fmt"
	"time"
)

func client(ch chan string, req string) (string, error) {
	ch <- req
	select {
	case ack := <-ch:
		return ack, nil
	case <-time.After(time.Second):
		return "", errors.New("timeout")
	}
}

func server(ch chan string) {
	for {
		req := <-ch
		fmt.Println("request:", req)
		ch <- "done"
	}
}

func main() {
	ch := make(chan string)
	go server(ch)
	for {
		res, err := client(ch, "req")
		if err != nil {
			fmt.Println("error")
		} else {
			fmt.Println("response:", res)
		}
	}
}
