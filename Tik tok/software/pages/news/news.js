Page({
  data: {
    renderData: [],
    loadingHidden:true,
  },
  onLoad: function () {
    this.getnews();
  },
    getnews:function(){
    var that = this//不要漏了这句，很重要
    wx.request({
      url: 'http://v.juhe.cn/toutiao/index', //这里填写你的接口路径，我们这里使用的是聚合数据api
      header: { //这里写你借口返回的数据是什么类型
        'Content-Type': 'application/json'
      },
      data: {//这里写你要请求的参数
        key: 'c68714cd657ab1ecd53b2087eb4089b1',
        type: 'top'
      },
      success: function (res) {
        console.log(res.data.result.data)
        //将获取到的json数据，存在这个数组中
        that.setData({
      
          renderData: res.data.result.data,
          //res代表success函数的事件对，data是固定的，result参见聚合数据返回文档
        });
        }   
      })

    },

  onShow: function () {
    // 页面显示
  },
  onHide: function () {
    // 页面隐藏
  },
  onUnload: function () {
    // 页面关闭
  },
  onPullDownRefresh() {}

})