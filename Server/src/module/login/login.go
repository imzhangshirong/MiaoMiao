package login

import(
	app "app"
	debug "package/debug"
	api "package/api"
)

func init(){

}

func Login(request *api.ApiRequest,response *api.ApiResponse){
	session:=app.SessionManager.GetFromClient(response.Writer,request.Request)
	defer func(){
		if session.Modified {
			app.SessionManager.Save(session)
		}
	}()
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

