package apiauth

import(
	"net/http"

	app "app"
	//debug "package/debug"
)

type Developer struct{
	Id int
	Developer string //开发者用户名
	Appid string //授权appid
	Dgroup int //用户组
}

func CheckAuth(request *http.Request) bool{
	appid:= request.Header.Get("appid")
	if appid != ""{
		datas,_ := app.DBUtil.Query(
			"SELECT id,developer,appid,dgroup FROM `developer` WHERE "+
			"`appid`='"+appid+"'",
			Developer{})
		if len(datas) > 0 {
			return true
		}
	}
	return false
}