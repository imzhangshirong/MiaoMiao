package debug

import(
	"fmt"
	"time"
)


func init(){

}

func print(format string,args ...interface{}){
	fmt.Printf(string(time.Now().Format("2006-01-02 15:04:05.000"))+" ")
	fmt.Printf(format,args...)
}

func Log(format string,args ...interface{}){
	print(format,args...)
	fmt.Printf("\n")
}

func Error(format string,args ...interface{}){
	fmt.Printf("\x1b[;;31m")
	print(format,args...)
	fmt.Printf("\x1b[0m\n")
}

func Warning(format string,args ...interface{}){
	fmt.Printf("\x1b[;;33m")
	print(format,args...)
	fmt.Printf("\x1b[0m\n")
}

func Info(format string,args ...interface{}){
	fmt.Printf("\x1b[;;36m")
	print(format,args...)
	fmt.Printf("\x1b[0m\n")
}