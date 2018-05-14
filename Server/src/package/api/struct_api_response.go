package api

import(
	"bytes"
	"net/http"
	"encoding/json"

	debug "package/debug"
)

type ApiResponse struct{
	Writer http.ResponseWriter
	Header *http.Header
	HttpCode int
	ErrorCode int
	Message string
	Data interface{}
	JsonData string
	BytesData *bytes.Buffer

}

type ApiResponseBase struct{
	Code int `json:"code"`
	Message string `json:"message"`
	Data interface{} `json:"data"`
}

func (p *ApiResponse) BuildJsonData() {
	/*if p.Data == nil {
		p.JsonData = ""
		return
	}*/
	rData := ApiResponseBase{p.ErrorCode,p.Message,p.Data}
	if rData.Code != 0 {
		rData.Data = nil
	}
	jsonStr,err := json.Marshal(rData)
	if err == nil {
		p.JsonData = string(jsonStr)
	}else{
		debug.Error(err.Error())
	}
}