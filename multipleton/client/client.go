package client

type Client struct{}

func NewClient() *Client {
	return &Client{}
}

func (c *Client) Close() { // 线程安全, 可重复调用, 调用之后不能再使用该类其他方法
}

func (c *Client) F() { // 其他什么方法
}
