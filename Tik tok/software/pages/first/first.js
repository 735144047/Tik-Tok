Page
({
    jump1: function () {
      wx.switchTab({
        url: '../index/index'
      })
    },//跳转至index页面

    jump2: function () {
      wx.switchTab({
        url: '../word/word'
      })
    },//跳转至word页面
    jump3: function () {
      wx.switchTab({
        url: '../news/news'
      })
    },//跳转至news页面
    jump4: function () {
      wx.switchTab({
        url: '../jishiben/jishiben'
      })
    },//跳转至jishiben页面
})