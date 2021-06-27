local UTILS = require "tests.test_utils"

return function()
    describe("BodyDef", function()
        before(function()
            UTILS.set_env(getfenv(1))
        end)
        after(function() end)


        test("index unknown", function()
            local world = box2d.NewWorld()
            local f = function()
               world:CreateBody({ unknown = true })
            end
            local status, value = pcall(f)
            assert_false(status)
            assert_equal(value , "unknown key:unknown")
            world:Destroy()
        end)


        test("type", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({type = box2d.b2BodyType.b2_kinematicBody})
            assert_equal(body:GetType(), box2d.b2BodyType.b2_kinematicBody)
            world:Destroy()
        end)
        test("position", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({position = vmath.vector3(2,2,0)})
            assert_equal_v3(body:GetPosition(), vmath.vector3(2,2,0))
            world:Destroy()
        end)
        test("angle", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({angle = math.pi})
            assert_equal_float(body:GetAngle(), math.pi)
            world:Destroy()
        end)
        test("linearVelocity", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({linearVelocity = vmath.vector3(2,2,0)})
            assert_equal_v3(body:GetLinearVelocity(), vmath.vector3(2,2,0))
            world:Destroy()
        end)

        test("angularVelocity", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({angularVelocity = math.pi})
            assert_equal_float(body:GetAngularVelocity(), math.pi)
            world:Destroy()
        end)

        test("linearDamping", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({linearDamping = 0.5})
            assert_equal_float(body:GetLinearDamping(), 0.5)
            world:Destroy()
        end)

        test("angularDamping", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({angularDamping = 0.5})
            assert_equal_float(body:GetAngularDamping(), 0.5)
            world:Destroy()
        end)

        test("allowSleep", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({allowSleep = false})
            assert_equal(body:IsSleepingAllowed(), false)
            body = world:CreateBody({allowSleep = true})
            assert_equal(body:IsSleepingAllowed(), true)
            world:Destroy()
        end)

        test("awake", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({type = box2d.b2BodyType.b2_dynamicBody, awake = false})
            assert_equal(body:IsAwake(), false)
            body = world:CreateBody({type = box2d.b2BodyType.b2_dynamicBody,awake = true})
            assert_equal(body:IsAwake(), true)
            world:Destroy()
        end)

        test("fixedRotation", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({fixedRotation = false})
            assert_equal(body:IsFixedRotation(), false)
            body = world:CreateBody({fixedRotation = true})
            assert_equal(body:IsFixedRotation(), true)
            world:Destroy()
        end)

        test("bullet", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({bullet = false})
            assert_equal(body:IsBullet(), false)
            body = world:CreateBody({bullet = true})
            assert_equal(body:IsBullet(), true)
            world:Destroy()
        end)

        test("enabled", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({bullet = false})
            assert_equal(body:IsBullet(), false)
            body = world:CreateBody({bullet = true})
            assert_equal(body:IsBullet(), true)
            world:Destroy()
        end)

        test("userData", function()
            local world = box2d.NewWorld()
            local f = function () world:CreateBody({userData = 222}) end
            local status, error = pcall(f)
            assert_false(status)
            assert_equal(error, "userdata can be only table or nil")

            local user_data = {}
            local body = world:CreateBody({userData = user_data})
            assert_equal(body:GetUserData(),user_data)

            world:Destroy()
        end)

        test("gravityScale", function()
            local world = box2d.NewWorld()
            local body = world:CreateBody({gravityScale = 2})
            assert_equal_float(body:GetGravityScale(), 2)
            world:Destroy()
        end)

        


    end)
end