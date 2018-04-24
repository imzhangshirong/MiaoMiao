package login

import(
	config "config"
	debug "package/debug"
	api "package/api"
)

func init(){

}

func Login(request *api.ApiRequest,response *api.ApiResponse){
	debug.Log("login")
	response.Header.Set("token","ac5d706b900c087f0e3")
	response.HttpCode = 303
	response.ErrorCode = -1
	response.Message = config.Error.GetErrorCodeMessage(response.ErrorCode)
}

