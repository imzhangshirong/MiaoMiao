package config

type ErrorConfig struct{
	errorInfo map[int]string
}


func (p *ErrorConfig) Init()(*ErrorConfig){
	p.errorInfo = make(map[int]string)

	p.errorInfo[-2] = "Api Auth Failed"
	p.errorInfo[-1] = "Server Error"
	p.errorInfo[0] = "Successed"
	p.errorInfo[1] = "Token Timeout"
	
	return p
}

func (p *ErrorConfig) GetErrorCodeMessage(code int) string{
	if _, exist := p.errorInfo[code]; !exist{
		return "Unknow Error"
	}
	return p.errorInfo[code]
}