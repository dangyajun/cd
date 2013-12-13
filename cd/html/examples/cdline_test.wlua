
require "cdlua"
require "iuplua"
require "iupluacd"

local debug_width = iup.label{ size = "50x", }
function debug_width:set(val)
    self.title = string.format("Width: %5i", val)
end

local debug_height = iup.label{ size = "50x", }
function debug_height:set(val)
    self.title = string.format("Height: %5i", val)
end

local debug_x = iup.label{ size = "50x", }
function debug_x:set(val)
    self.title = string.format("X: %5i", val)
end

local debug_y1 = iup.label{ size = "50x", }
function debug_y1:set(val)
    self.title = string.format("Y: %5i", val)
end

local debug_y2 = iup.label{ size = "50x", }
function debug_y2:set(val)
    self.title = string.format("Y: %5i", val)
end

local debug_write_mode = iup.label{ size = "100x", }
function debug_write_mode:set(val)
    self.title = string.format("WriteMode: %s", val)
end

local debug_line_color = iup.label{ size = "100x", }
function debug_line_color:set(val)
    self.title = string.format("COLOR: r:%i g:%i b:%i", cd.DecodeColor(val))
end

local debug_line_join = iup.label{ size = "100x", }
function debug_line_join:set(val)
    self.title = string.format("LineJoin: %i", val)
end

local debug_line_cap = iup.label{ size = "100x", }
function debug_line_cap:set(val)
    self.title = string.format("LineCap: %i", val)
end

local cnv = iup.canvas {
    rastersize = "x24",
    expand = "HORIZONTAL",
    --border = "NO",

    -- user-defined attrs
    line_width = 1,
    write_mode = cd.XOR,
    line_join  = cd.MITER,
    line_cap   = cd.CAPFLAT,
    line_color = cd.EncodeColor(0, 255, 255),
}

function cnv:map_cb()
    self.canvas = cd.CreateCanvas(cd.IUP, self)
end

-- draw vertical bar
function cnv:draw_vline(x)
    local c = self.canvas
    c:Activate()
    local w,h = c:GetSize()
    debug_width:set(w)
    debug_height:set(h)
    
    local y1, y2 = 0, h-1

    debug_x:set(x)
    debug_y1:set(y1)
    debug_y2:set(y2)

    c:WriteMode(self.write_mode)
    c:LineWidth(self.line_width)
    c:Foreground(self.line_color)
    c:LineJoin(self.line_join)
    c:LineCap(self.line_cap)

    debug_line_color:set(c:Foreground(cd.QUERY))
    debug_write_mode:set(c:WriteMode(cd.QUERY))
    debug_line_join:set(c:LineJoin(cd.QUERY))
    debug_line_cap:set(c:LineCap(cd.QUERY))

    c:Line(x,y1,x,y2)
    c:Deactivate()
end

function cnv:move_vline(x)
    local w,h = self.canvas:GetSize()
    if x >= 0 and x < w then
        local _x = self.old_x or x
        if _x ~= x then
            self:draw_vline(_x)
            _x = x
        end
        self:draw_vline(x)
        self.old_x = _x
    end
end

function cnv:button_cb(but, pressed, x, y, status)
    if but == iup.BUTTON1
    then
        self.old_x = pressed == 1 and x
        self:draw_vline(x)
    end
end

function cnv:motion_cb(x, y, status)
    if status:sub(3,3) == "1" then
        self:move_vline(x)
    end
end



local dlg = iup.dialog {
    iup.frame{
        iup.vbox{
            margin = "2x2",

            -- canvas
            iup.hbox{
                cnv,
            },

            iup.hbox{
                iup.frame{
                    title = "LineWidth",
                    iup.vbox{
                        margin = "10x",
                        iup.fill{},
                        iup.text{
                            visiblecolumns = 2,
                            spin = "YES",
                            spinmin = 1,
                            spin_cb = function (self, pos)
                                cnv.line_width = pos
                            end,
                        },
                        iup.fill{},
                    },
                },
                iup.frame{
                    title = "WriteMode",
                    iup.radio{
                        iup.vbox{
                            iup.toggle{
                                title="XOR",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.write_mode = cd.XOR
                                    end
                                end,
                            },
                            iup.toggle{
                                title="REPLACE",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.write_mode = cd.REPLACE
                                    end
                                end,
                            },
                            iup.fill{},
                         },
                    },
                },
                iup.frame{
                    title = "LineJoin",
                    iup.radio{
                        iup.vbox{
                            iup.toggle{
                                title="MITER",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.line_join = cd.MITER
                                    end
                                end,
                            },
                            iup.toggle{
                                title="BEVEL",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.line_join = cd.BEVEL
                                    end
                                end,
                            },
                            iup.toggle{
                                title="ROUND",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.line_join = cd.ROUND
                                    end
                                end,
                            },
                            iup.fill{},
                         },
                    },
                },
                iup.frame{
                    title = "LineCap",
                    iup.radio{
                        iup.vbox{
                            iup.toggle{
                                title="CAPFLAT",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.line_cap = cd.CAPFLAT
                                    end
                                end,
                            },
                            iup.toggle{
                                title="CAPSQUARE",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.line_cap = cd.CAPSQUARE
                                    end
                                end,
                            },
                            iup.toggle{
                                title="CAPROUND",
                                action = function (self, state)
                                    if state == 1 then
                                        cnv.line_cap = cd.CAPROUND
                                    end
                                end,
                            },
                            iup.fill{},
                        },
                    },
                },
                iup.frame{
                    title = "Debug",
                    iup.hbox{
                        iup.vbox{
                            debug_width,
                            debug_height,
                        },
                        iup.vbox{
                            debug_x,
                            debug_y1,
                            debug_y2,
                        },
                        iup.vbox{
                            debug_line_color,
                            debug_write_mode,
                            debug_line_join,
                            debug_line_cap,
                        },
                    },
                },
            },
        },
    },
    size = "HALFx",
}

dlg:showxy(iup.CENTER, iup.CENTER)

iup.MainLoop()

