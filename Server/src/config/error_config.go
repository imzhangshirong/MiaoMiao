package config

type ErrorConfig struct{
	errorInfo map[int]string
}


func (p *ErrorConfig) Init()(*ErrorConfig){
	p.errorInfo = make(map[int]string)

	p.errorInfo[0] = "Successed"
	p.errorInfo[-1] = "Error!!!!!!"
	
	return p
}

func (p *ErrorConfig) GetErrorCodeMessage(code int) string{
	if _, exist := p.errorInfo[code]; !exist{
		return "Unknow Error"
	}
	return p.errorInfo[code]
}