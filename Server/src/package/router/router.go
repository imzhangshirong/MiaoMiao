package router

import(
	"fmt"
	"net/http"
	"strconv"

	"github.com/gorilla/mux"
	config "config"
	api "package/api"


)

type Router struct{
	router *mux.Router
	host string
	port int
}


func (p *Router) AddHandle(path string,handle func(*api.ApiRequest,*api.ApiResponse)) {
	p.router.HandleFunc(path,func(writer http.ResponseWriter,request *http.Request){
		resHeader := writer.Header()
		apiRequest := api.ApiRequest{Request:request}
		apiResponse := api.ApiResponse{Writer:writer,Header:&resHeader,HttpCode:0,ErrorCode:0,Message:"",Data:nil,JsonData:"",BytesData:nil}
		handle(&apiRequest,&apiResponse)
		if apiResponse.HttpCode == 0 {
			apiResponse.HttpCode = 502
			apiResponse.ErrorCode = -1
		}
		if apiResponse.Message == "" {
			apiResponse.Message = config.Error.GetErrorCodeMessage(apiResponse.ErrorCode)
		}
		apiResponse.BuildJsonData()
		writer.WriteHeader(apiResponse.HttpCode)
		if apiResponse.BytesData==nil{
			writer.Write([]byte(apiResponse.JsonData))
		}else if apiResponse.BytesData!=nil && apiResponse.BytesData.Len()>0{
			writer.Write(apiResponse.BytesData.Bytes())
		}
	})
}


func (p *Router) Init(host string,port int)(*Router){
	p.host = host
	p.port = port
	p.router = mux.NewRouter().StrictSlash(true)
	return p
}

func (p *Router) Listen(){
	var addr string = p.host+":"+strconv.Itoa(p.port)
	error:=http.ListenAndServe(addr,p.router)
	if error != nil {
		fmt.Println(error.Error())
	}
}