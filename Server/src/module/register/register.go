package register

import(
	"image/jpeg"
	"bytes"

	app "app"
	debug "package/debug"
	api "package/api"
	captcha "package/captcha"

	module_apiauth "module/apiauth"
	
)

func init(){

}

func OnCaptcha(request *api.ApiRequest,response *api.ApiResponse){
	session:=app.SessionManager.GetFromClient(response.Writer,request.Request)
	captcha,str := captcha.GetCaptcha()
	cstr:=session.Get("captcha").(string)
	debug.Log("last:"+cstr)
	session.Set("captcha",str)
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
	response.HttpCode = 200
	response.ErrorCode = 0
	response.BytesData = new(bytes.Buffer)
	jpeg.Encode(response.BytesData,captcha, nil)
}

