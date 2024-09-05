package multipleton

import (
	"github.com/zweix123/code-examples/multipleton/client"
	"github.com/zweix123/code-examples/multipleton/multiple"
)

var (
	multipleton *client.Client
	mp          multiple.Multiple
)

func GetInstance() *client.Client {
	mp.Do(func() {
		multipleton = client.NewClient()
	})
	return multipleton
}

func CloseInstance() {
	multipleton.Close()
	// 没有对 multipleton 的重新置空
	// 1. 没必要, 其他的地方也没有判断
	// 2. 会报错, multiTime 已经 Renew 了, 可能已经有其他线程在调用 GetInstance 了
	mp.Renew()
	// 假如在 CloseInstance 之后, 又有线程调用了 GetInstance, 那么会再次创建一个新的 client.Client
	// 但是也不能不加, 因为假如不加还又有线程调用了 GetInstance, 那么可能造成这个线程对一个 close 的 client.Client 进行操作, 更不行
}

func ReStartInstance() {
	multipleton.Close() // client.Client.Close 是可以重复调用不会报错的, 所以在这里不会有问题
	mp.Renew()
}
