package main

import (
    "fmt"
    "net/http"
    "os"
)

func pid(w http.ResponseWriter, req *http.Request) {

    fmt.Fprintf(w, "pid: %d\n", os.Getpid())
    hn, err := os.Hostname()
    if err != nil {
		fmt.Fprintf(w, "error with getting hostname\n")
	} else {
        fmt.Fprintf(w, "%s\n",hn)
	}
}

func headers(w http.ResponseWriter, req *http.Request) {

    for name, headers := range req.Header {
        for _, h := range headers {
            fmt.Fprintf(w, "%v: %v\n", name, h)
        }
    }
}

func root(w http.ResponseWriter, reg *http.Request) {
    fmt.Fprintf(w, "it's root\n");
}

func main() {

    http.HandleFunc("/pid", pid)
    http.HandleFunc("/headers", headers)
    http.HandleFunc("/", root)

    http.ListenAndServe(":8099", nil)
}
