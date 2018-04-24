package router

import(
	"fmt"
	"net/http"
	"strconv"

	"github.com/gorilla/mux"

	api "package/api"


)

type Router struct{
	router *mux.Router
}


func (p *Router) AddHandle(path string,handle func(*api.ApiRequest,*api.ApiResponse)) {
	p.router.HandleFunc(path,func(writer http.ResponseWriter,request *http.Request){
		resHeader := writer.Header()
		apiRequest := api.ApiRequest{Request:request}
		apiResponse := api.ApiResponse{Header:&resHeader,HttpCode:200,ErrorCode:0,Message:"",Data:nil,JsonData:""}
		handle(&apiRequest,&apiResponse)
		apiResponse.BuildJsonData()
		writer.WriteHeader(apiResponse.HttpCode)
		writer.Write([]byte(apiResponse.JsonData))
	})
}


func (p *Router) Init()(*Router){
	p.router = mux.NewRouter().StrictSlash(true)
	return p
}

func (p *Router) Listen(host string,port int){
	var addr string = host+":"+strconv.Itoa(port)
	error:=http.ListenAndServe(addr,p.router)
	if error != nil {
		fmt.Println(error.Error())
	}
}