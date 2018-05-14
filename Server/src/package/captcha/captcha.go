package captcha

import(
	//"os"
    //"image/jpeg"

	//debug "package/debug"
)

var config ConfigDigit = ConfigDigit{
    Width:200,
    Height:80,
    MaxSkew:0.8,
    DotCount:80,
    CaptchaLen:6}

func GetCaptcha()(*CaptchaImageDigit,string){
    img,str:= EngineDigitsCreate()
    return img,str
}

func init(){

}
