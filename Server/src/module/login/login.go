package login

import(
	app "app"
	debug "package/debug"
	api "package/api"

	module_apiauth "module/apiauth"
	module_token "module/token"
)

func init(){

}

func OnLogin(request *api.ApiRequest,response *api.ApiResponse){
	session:=app.SessionManager.GetFromClient(response.Writer,request.Request)
	defer func(){
		if session.Modified {
			app.SessionManager.Save(session)
		}
	}()
	if !module_apiauth.CheckAuth(request.Request) {
		response.HttpCode = 200
		response.ErrorCode = -2
		return
	}
	if !module_token.CheckToken(request.Request) {
		response.HttpCode = 200
		response.ErrorCode = 1
		return
	}

	var name string
	if v := session.Get("name");v!=nil{
		name = v.(string)
		session.Set("name",name + "hello")
	}else{
		session.Set("name","hello")
	}
	debug.Log("login:"+session.Sid)
	response.HttpCode = 303
	response.ErrorCode = 0
	response.Message = name
}

