Page({
  data: {
    csnum: "",
    lfnum: "",
    fynum: "",
    flag1: true,
    flag2: true,
    flag3: true,
    flag4: true,
    t: "",
    h: "",
    num1:"",
    num2:"",
    num3:"",
  },

  getdata: function () {
    let that = this;
    const requestTask = wx.request({
      url: 'https://api.heclouds.com/devices/505040475/datapoints?datastream_id=Temperature,Humidity,pirSupermarket,pirBarberShop,pirCopyShop&limit=1',
      header: {
        'content-type': 'application/json',
        'api-key': 'F5eDYem6rDzxPrxILebj21vprxk='
      },
      success: function (res) {
        //console.log('api:', res.data.data)
        var app = getApp();
        app.globalData.temperature = res.data.data.datastreams[0].datapoints[0].value;
        app.globalData.humidity = res.data.data.datastreams[1].datapoints[0].value;
        app.globalData.csnum = res.data.data.datastreams[2].datapoints[0].value;
        app.globalData.lfnum = res.data.data.datastreams[3].datapoints[0].value;
        app.globalData.fynum = res.data.data.datastreams[4].datapoints[0].value;
        // console.log(app.globalData.temperature) 
        //console.log(app.globalData.csnum)    测试是否赋值成功

        that.setdata();//微信小程序是异步的，调用数据没完成时就继续往下进行，所以要把赋值函数放在调用函数里面
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      },


    })
  },
  //赋值函数
  setdata: function () {
    var that = this;
    var app = getApp()
    //console.log(app.globalData.fynum)
    that.setData({
      t: app.globalData.temperature,
      h: app.globalData.humidity,
    num1: app.globalData.csnum,
    num2: app.globalData.lfnum,
    num3: app.globalData.fynum,
  })
   // console.log(this.data.num2)
  },
  onLoad: function () {
    this.getdata();
    console.log(this.data.t)
  },
  
  //实现同时只显示一个地方的信息
changedata1:function(){
this.setData({
  flag1:false,
  flag2:true,
  flag3:true,
  flag4:true,
})},
changedata2: function() {
      this.setData({
        flag1: true,
        flag2: false,
        flag3: true,
        flag4: true,
      })},
changedata3: function() {
          this.setData({
            flag1: true,
            flag2: true,
            flag3: false,
            flag4: true,
          })},
changedata4: function() {
this.setData({
  flag1: true,
  flag2: true,
  flag3: true,
  flag4: false,
              })
},

})