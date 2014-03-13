#bitmap = Bitmap.new("bitmap/test.png", nil)
#bitmap.dispose
#sprite = Sprite.new
#sprite.dispose
def p(*arg)
  cp arg.to_s
end

Brush_Manager.create_brush(10, [0x66ccff, 1])
color = [0, 1.0, 0.0, 0xFFFFFF, 1.0, 1.1]
Brush_Manager.create_brush(11, color, 0, [0,0,100,100])


p Font_Manager.create_font(2, "微软雅黑", 100)
p Font_Manager.create_font(20, "楷体", 100)


sprite = Sprite.new
sprite.bitmap = Bitmap.new(200, 200)
sprite.bitmap.draw_text(0, 0, 200, 200, "Hello,world!")
sprite2 = Sprite.new
sprite2.x = 100.0
sprite2.y = 10.0
sprite2.bitmap = sprite.bitmap.dup
1000.times{
  sprite.x += 0.04
  sprite.y += 0.03
  Graphics.update
}
sprite.bitmap.dispose
sprite.dispose