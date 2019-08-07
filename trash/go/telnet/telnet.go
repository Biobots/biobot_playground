package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

func processCommand(str string, exitCh chan int) bool {
	if strings.HasPrefix(str, "@close") {
		fmt.Println("Session closed")
		return false
	} else if strings.HasPrefix(str, "@shutdown") {
		exitCh <- 0
		return false
	}
	fmt.Println(str)
	return true
}

func handleSession(conn net.Conn, exitCh chan int) {
	fmt.Println("Session start")
	reader := bufio.NewReader(conn)
	for {
		str, err := reader.ReadString('\n')
		if err == nil {
			str = strings.TrimSpace(str)
			if !processCommand(str, exitCh) {
				conn.Close()
				break
			}
			conn.Write([]byte(str + "\r\n"))
		} else {
			fmt.Println("Session error")
			conn.Close()
			break
		}
	}
}

func server(address string, exitCh chan int) {
	l, err := net.Listen("tcp", address)
	if err != nil {
		fmt.Println(err.Error())
		exitCh <- 1
	}
	fmt.Println("listen:", address)
	defer l.Close()
	for {
		conn, err := l.Accept()
		if err != nil {
			fmt.Println(err.Error())
			continue
		}
		go handleSession(conn, exitCh)
	}
}

func main() {
	exitCh := make(chan int)
	go server("127.0.0.1:7001", exitCh)
	code := <-exitCh
	os.Exit(code)
}
