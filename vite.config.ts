import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// NOTE: If you will host at https://<username>.github.io/Visual_without_ros,
// set base to '/Visual_without_ros/'. If you host at a user/site root page, set base to '/'.
export default defineConfig({
  base: '/Visual_without_ros/',
  plugins: [react()]
})
