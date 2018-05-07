package token

import(
	"net/http"

	app "app"
	//debug "package/debug"
)

type UserToken struct{
	Id int
	Token string

}

func CheckToken(request *http.Request) bool{
	token:= request.Header.Get("token")
	if token != ""{
		datas,_ := app.DBUtil.Query(
			"SELECT id,token FROM `user` WHERE "+
			"`token`='"+token+"'",
			UserToken{})
		if len(datas) > 0 {
			return true
		}
	}
	return false
}