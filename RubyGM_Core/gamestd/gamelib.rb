class Bitmap
  def width
    return @_edata_[1]
  end
  def height
    return @_edata_[2]
  end
  def font
    return @_edata_[3]
  end
  def brush
    return @_edata_[4]
  end
  def disposed?
    return @_edata_[5]
  end
end
class Sprite
  def x
    return @_edata_[2]
  end
  def x=(new_x)
    @_edata_[2] = new_x
    @_edata_[1] = true
    return new_x
  end
  def y
    return @_edata_[3]
  end
  def y=(new_y)
    @_edata_[3] = new_y
    @_edata_[1] = true
    return new_y
  end
  def bitmap
    return @bitmap
  end
  def disposed?
    return @_edata_[5]
  end
end